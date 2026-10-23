/*
 * A044167  Numbers k such that string 4,1 occurs in the base 7 representation
 *          of k but not of k-1.
 * https://oeis.org/A044167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 1500, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = nth integer k such that the base‑7 representation of k
   contains the substring "41" while k−1 does not.
   The function searches sequentially from the previously found
   value and stores terms in a growable static array. */

#include <stdlib.h>
#include <stdbool.h>

/* Return true if x in base 7 contains the consecutive digits 4,1. */
static bool has41(seq_t x)
{
    seq_t t = x;
    seq_t prev = 0;          /* digit to the right (less significant) */
    bool result = false;

    while (t > 0u) {
        seq_t cur = t % 7u;   /* current digit (more significant now) */
        if (cur == 4u && prev == 1u)   /* pattern 4 followed by 1 */
            result = true;
        prev = cur;
        t /= 7u;
    }
    return result;
}

/* Return the nth term of the sequence (1‑based indexing). */
seq_t a(idx_t n)
{
    static seq_t *seq   = NULL;     /* stored terms */
    static idx_t seq_len  = 0;      /* number of terms stored */
    static idx_t seq_cap  = 0;      /* allocated capacity */
    static seq_t last_k = 0;        /* last found term */

    if (n == 0u)  /* undefined for index 0; return 0 (unused) */
        return 0;

    /* Generate terms until we reach the desired index. */
    while (seq_len < n) {
        seq_t cand = last_k + 1u;      /* start searching from the next integer */

        for (;;) {
            if (has41(cand) && !has41(cand - 1u)) {
                /* Grow the array if needed. */
                if (seq_len == seq_cap) {
                    idx_t new_cap = seq_cap ? seq_cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                    if (!tmp) exit(EXIT_FAILURE);  /* out of memory */
                    seq = tmp;
                    seq_cap = new_cap;
                }
                seq[seq_len] = cand;
                seq_len++;
                last_k = cand;
                break;
            }
            ++cand;                /* try next integer */
        }
    }
    return seq[(size_t)(n - 1u)];
}
