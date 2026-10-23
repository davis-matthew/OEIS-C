/*
 * A108158  Triangular numbers whose 10's complements are squares.
 * https://oeis.org/A108158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the n‑th triangular number T such that (10^d - T) is a
 *            perfect square, where d = number of decimal digits of T
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 19, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isqrt.h"

/* Compute the n-th term (n ≥ 1). */
seq_t a(idx_t n)
{
    /* Memoised results. */
    static seq_t *terms = NULL;
    static size_t terms_cap = 0;
    static size_t terms_cnt = 0;
    static idx_t last_m = 0;        /* last m for which T = m(m+1)/2 was examined */

    /* Table of 10^d, where d runs from 0 upward. */
    static seq_t *pow10 = NULL;
    static size_t pow_cap = 0, pow_cnt = 0;   /* pow_cnt = number of valid entries in pow10 */

    /* Initialise the powers-of-10 table once. */
    if (pow_cnt == 0) {
        pow_cap = 16;
        pow10 = (seq_t *)malloc(pow_cap * sizeof(seq_t));
        pow_cnt = 1;               /* pow10[0] = 1 (=10^0) */
        pow10[0] = (seq_t)1;
    }

    /* Extend the cached list of terms until we have at least n entries. */
    while (terms_cnt < n) {
        /* Next triangular number. */
        ++last_m;
        seq_t m = (seq_t)last_m;
        seq_t T = m * (m + 1) / 2;

        /* Grow the 10^d table if T has more digits than the current largest power. */
        while (T >= pow10[pow_cnt - 1]) {
            if (pow_cnt == pow_cap) {
                pow_cap <<= 1;
                pow10 = (seq_t *)realloc(pow10, pow_cap * sizeof(seq_t));
            }
            pow10[pow_cnt] = pow10[pow_cnt - 1] * (seq_t)10;
            ++pow_cnt;
        }

        /* Complement with respect to the smallest power of 10 that exceeds T. */
        seq_t p10  = pow10[pow_cnt - 1];
        seq_t comp = p10 - T;

        /* Is the complement a perfect square? */
        seq_t s = isqrt(comp);
        if (s * s == comp) {
            /* Store the qualifying triangular number. */
            if (terms_cnt == terms_cap) {
                terms_cap = terms_cap ? terms_cap * 2 : 16;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_cnt++] = T;
        }
    }

    return terms[n - 1];
}
