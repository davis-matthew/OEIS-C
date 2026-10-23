/*
 * A193134  Numbers of spanning trees of the folded cube graphs.
 * https://oeis.org/A193134
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, more, changed
 * Formula:   a(n) = 2^{2^{m}-1-m} * ∏_{k=1}^{m} g(k)^{C(m,k)} where m = n-1
 *            and g(k) = k if k is even, k+1 if k is odd
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This computes the number of spanning trees of the folded cube graph FQ_{m}.  */
/* For n=2 (m=1) the graph is a single edge, so a(2)=1.                */
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;          /* not part of the sequence */
    if (n == 2) return (seq_t)1;         /* FQ_1 has one spanning tree */

    idx_t m = n - 1;                     /* dimension of the folded cube */

    /* 2^{2^m - 1 - m}  factor */
    seq_t two_power_m = big_pow((seq_t)2, (seq_t)m);          /* 2^m */
    seq_t exponent_two = two_power_m - (seq_t)1 - (seq_t)m;   /* 2^m-1-m */
    seq_t result = big_pow((seq_t)2, exponent_two);          /* 2^{2^m-1-m} */

    /* C(m,k) accumulation and multiplication of terms */
    seq_t binom = (seq_t)1;                /* C(m,0) = 1 */
    for (idx_t k = 1; k <= m; ++k)
    {
        /* binom = C(m,k) */
        binom = (binom * (m - k + 1)) / k;

        /* base = g(k) */
        seq_t base = (k & 1) ? (seq_t)(k + 1) : (seq_t)k;

        /* multiply by base^{binom} */
        seq_t term = big_pow(base, binom);
        result *= term;
    }

    return result;
}
