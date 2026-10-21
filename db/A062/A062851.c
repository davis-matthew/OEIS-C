/*
 * A062851  Number of k such that 1 < k < n X n and k not of the form ij for 1
 *          <= {i, j} <= n.
 * https://oeis.org/A062851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2 - D(n) where D(n) is the number of distinct integers
 *            that can be expressed as i*j with 1 ≤ i,j ≤ n. The function
 *            keeps a bitset of all products and counts the number of distinct
 *            values
 * Range:     u8 n <= 20, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;        // offset protection

    /* ------- static state ------------------------------------------ */
    static seq_t *a_vals   = NULL;   /* a_vals[0] = a(1)              */
    static size_t a_len   = 0;       /* how many values are filled   */
    static size_t a_cap   = 0;       /* allocated capacity           */

    static uint64_t *bitset = NULL;  /* bitset for products          */
    static size_t bit_cap_words = 0; /* words allocated              */

    static uint64_t distinct_cnt = 0ULL;   /* distinct products seen */

    /* ------- handle cached values --------------------------------- */
    if ((size_t)n <= a_len) {
        return a_vals[n - 1];
    }

    /* ------- ensure capacity for answer array --------------------- */
    if (n > a_cap) {
        size_t new_cap = a_cap ? a_cap * 2 : 16;
        if (new_cap < (size_t)n) new_cap = (size_t)n;
        seq_t *tmp = (seq_t*)realloc(a_vals, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failure */
        a_vals = tmp;
        a_cap = new_cap;
    }

    /* ------- extend tables up to n ------------------------------- */
    for (idx_t new_n = (idx_t)a_len + 1; new_n <= n; ++new_n) {
        uint64_t max_val = (uint64_t)new_n * (uint64_t)new_n;
        size_t needed = (size_t)((max_val + 63ULL) >> 6); /* words needed */

        if (needed > bit_cap_words) {
            size_t new_cap = bit_cap_words ? bit_cap_words * 2 : 32;
            if (new_cap < needed) new_cap = needed;
            uint64_t *tmp = (uint64_t*)realloc(bitset, new_cap * sizeof(uint64_t));
            if (!tmp) return (seq_t)0;  /* allocation failure */
            if (bitset) {
                /* zero just the newly allocated area */
                memset(tmp + bit_cap_words, 0,
                       (new_cap - bit_cap_words) * sizeof(uint64_t));
            } else {
                memset(tmp, 0, new_cap * sizeof(uint64_t));
            }
            bitset = tmp;
            bit_cap_words = new_cap;
        }

        /* add all products involving new_n (i * new_n) */
        for (idx_t i = 1; i <= new_n; ++i) {
            uint64_t prod = (uint64_t)i * (uint64_t)new_n;
            uint64_t idx  = prod - 1ULL;           /* zero‑based index   */
            size_t word   = idx >> 6;
            uint64_t mask = 1ULL << (idx & 63ULL);
            if ((bitset[word] & mask) == 0) {
                bitset[word] |= mask;
                distinct_cnt++;
            }
        }

        /* compute a(new_n) = new_n^2 - distinct_cnt   */
        seq_t val = (seq_t)((seq_t)new_n * (seq_t)new_n - (seq_t)distinct_cnt);
        a_vals[a_len++] = val;
    }

    return a_vals[n - 1];
}
