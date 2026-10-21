/*
 * A092549  Molien series for symmetrized weight enumerators of self-dual
 *          codes over GF(4) + GF(4)u with u^2 = 0.
 * https://oeis.org/A092549
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 56, u32 n <= 545, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence based on the generating function denominator:
 *   (1+x^5+x^8+x^13)/((1-x)*(1-x^2)^2*(1-x^4)^2*(1-x^6))
 * a(n) satisfies the linear recurrence
 *   a(n) = N[n] - Σ_{k=1}^{19} d[k] * a(n-k)
 * where N[n] = 1 if n∈{0,5,8,13} else 0,
 * and d[0..19] are the coefficients of D(x):
 *   D(x) = (1-x)*(1-x^2)^2*(1-x^4)^2*(1-x^6)
 * The sequence starts a(0)=1, a(1)=1, …
 * The implementation caches computed values so that
 * arbitrary order of calls is handled efficiently. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Coefficients of the denominator polynomial D(x) = (1-x)*(1-x^2)^2*(1-x^4)^2*(1-x^6)   */
    static const int d[20] = {
        1,   /* d[0]  */
       -1,   /* d[1]  */
       -2,   /* d[2]  */
        2,   /* d[3]  */
       -1,   /* d[4]  */
        1,   /* d[5]  */
        3,   /* d[6]  */
       -3,   /* d[7]  */
        1,   /* d[8]  */
       -1,   /* d[9]  */
       -1,   /* d[10] */
        1,   /* d[11] */
       -3,   /* d[12] */
        3,   /* d[13] */
        1,   /* d[14] */
       -1,   /* d[15] */
        2,   /* d[16] */
       -2,   /* d[17] */
       -1,   /* d[18] */
        1    /* d[19] */
    };
    /* Numerator coefficients of N(x) = 1 + x^5 + x^8 + x^13 */
    static const int N[14] = {
        1, /* N[0] */
        0, /* N[1] */
        0, /* N[2] */
        0, /* N[3] */
        0, /* N[4] */
        1, /* N[5] */
        0, /* N[6] */
        0, /* N[7] */
        1, /* N[8] */
        0, /* N[9] */
        0, /* N[10] */
        0, /* N[11] */
        0, /* N[12] */
        1  /* N[13] */
    };
    static seq_t *a_seq = NULL;   /* cached sequence values */
    static size_t a_cap = 0;      /* capacity of a_seq */
    static size_t a_len = 0;      /* number of values currently stored */

    /* Ensure capacity for at least n+1 elements */
    if (n + 1 > a_cap) {
        size_t new_cap = a_cap ? a_cap * 2 : 16;
        if (new_cap < (size_t)(n + 1))
            new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(a_seq, new_cap * sizeof(seq_t));
        a_seq = tmp;
        a_cap = new_cap;
    }

    /* Fill in missing values up to n */
    for (size_t i = a_len; i <= (size_t)n; ++i) {
        long long val = 0;  /* signed intermediate; safe for n up to 10000 */
        if (i < 14)
            val += N[i];
        for (int k = 1; k <= 19 && (size_t)k <= i; ++k) {
            seq_t term = a_seq[i - k];
            if (d[k] < 0) {
                val += (long long)(-d[k]) * (long long)term;
            } else {
                val -= (long long)d[k] * (long long)term;
            }
        }
        a_seq[i] = (seq_t)val;
    }

    a_len = (size_t)n + 1;
    return a_seq[n];
}
