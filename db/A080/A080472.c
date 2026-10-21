/*
 * A080472  a(n) = smallest triangular number that is obtained by placing
 *          digits anywhere in n; a(n) = n if n is a triangular number.
 * https://oeis.org/A080472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest triangular number whose decimal digits contain *
 *            the decimal digits of n as a subsequence (in the same order)
 * Range:     u8 n <= 13, u16 n <= 173, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   subseq_match
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/subseq_match.h"

/* Max digits for an arbitrary‑precision seq_t (up to ~39 for 128‑bit). */
#define MAX_DGT 40

/* Convert a seq_t value to its decimal digit array (most significant first). */
static void seq_to_digits(seq_t v, unsigned char d[MAX_DGT], int *len)
{
    if (v == (seq_t)0) {
        d[0] = 0;
        *len = 1;
        return;
    }
    int i = 0;
    while (v != (seq_t)0) {
        d[i++] = (unsigned char)(v % (seq_t)10);
        v /= (seq_t)10;
    }
    /* Reverse to most–significant–digit first. */
    for (int j = 0; j < i / 2; ++j) {
        unsigned char t = d[j];
        d[j] = d[i - 1 - j];
        d[i - 1 - j] = t;
    }
    *len = i;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Convert the index n to its decimal digit array. */
    unsigned char need[MAX_DGT];
    int need_len;
    seq_to_digits((seq_t)n, need, &need_len);

    seq_t tri = 0;            /* Current triangular number T_k. */
    unsigned char cand[MAX_DGT];
    int cand_len;

    /* Scan triangular numbers in increasing order. */
    for (idx_t k = 1; ; ++k) {
        tri += (seq_t)k;          /* T_k = T_{k-1} + k. */
        seq_to_digits(tri, cand, &cand_len);
        if (subseq_match(need, need_len, cand, cand_len))
            return tri;
    }
}
