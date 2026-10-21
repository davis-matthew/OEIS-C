/*
 * A051246  Binary numbers d(1)...d(j) such that d(i) != d(j+1-i) for all but
 *          4 values of i.
 * https://oeis.org/A051246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 5630, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A051246: binary numbers with exactly four positions i such that
 * d(i) == d(j+1-i) (i.e. exactly two symmetric pairs are equal). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Test whether a number of length len has exactly four equal indices. */
static bool is_valid(uint64_t num, uint64_t len)
{
    uint64_t eq = 0;
    for (uint64_t i = 0; i < len; ++i) {
        uint64_t bit1 = (num >> i) & 1ULL;
        uint64_t bit2 = (num >> (len - 1 - i)) & 1ULL;
        if (bit1 == bit2)
            ++eq;
    }
    return eq == 4;
}

/* External linkage as required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */

    static seq_t* seq   = NULL;     /* array of already generated terms */
    static size_t seq_len  = 0;     /* number of elements stored */
    static size_t seq_cap  = 0;     /* current capacity of the array */

    /* Length of binary strings to examine next; start from 4 bits. */
    static uint64_t next_len = 4;

    while (seq_len < (size_t)n) {
        uint64_t j = next_len;
        if (j < 4) j = 4;
        if (j & 1) j++;              /* keep even length only */
        next_len = j + 2;            /* prepare for the following length */

        uint64_t start = 1ULL << (j - 1);     /* msb must be 1 */
        uint64_t end   = 1ULL <<  j;          /* exclusive upper bound */

        /* First pass: count how many valid numbers of this length. */
        size_t count = 0;
        for (uint64_t num = start; num < end; ++num) {
            if (is_valid(num, j)) ++count;
        }
        if (count == 0) continue;          /* nothing to add for this j */

        /* Allocate a temporary array large enough to hold all of them. */
        seq_t* tmp = (seq_t*)malloc(count * sizeof(seq_t));
        if (!tmp) return 0;                /* allocation failed */

        /* Second pass: store the numbers in ascending order
         * (iteration over num already guarantees sorted order). */
        size_t k = 0;
        for (uint64_t num = start; num < end; ++num) {
            if (is_valid(num, j)) {
                tmp[k++] = (seq_t)num;
            }
        }

        /* Append the generated term(s) to the global list. */
        if (seq_len + count > seq_cap) {
            size_t new_cap = seq_cap ? seq_cap * 2 : count * 2;
            if (new_cap < seq_len + count) new_cap = seq_len + count;
            seq_t* new_seq = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
            if (!new_seq) {
                free(tmp);
                return 0;                   /* allocation failed */
            }
            seq = new_seq;
            seq_cap = new_cap;
        }
        for (size_t i = 0; i < count; ++i) {
            seq[seq_len++] = tmp[i];
        }

        free(tmp);
    }

    return seq[n - 1]; /* zero‑based index */
}
