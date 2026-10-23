/*
 * A038230  Triangle whose (i,j)-th entry is binomial(i,j)*3^(i-j)*12^j.
 * https://oeis.org/A038230
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j) * 3^(i-j) * 12^j * where i =
 *            floor((sqrt(8*n+1)-1)/2), * j = n - i*(i+1)/2
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the row index i for a given n (0‑based) using binary search. */
static inline idx_t row_of_n(idx_t n)
{
    idx_t low  = 0;
    idx_t high = 1;

    /* Find an upper bound for i */
    while ((__uint128_t)high * (high + 1u) / 2u <= n)
        high <<= 1;

    /* Binary search for the largest i such that i*(i+1)/2 <= n */
    while (low < high) {
        idx_t mid = low + ((high - low + 1u) >> 1);
        if ((__uint128_t)mid * (mid + 1u) / 2u <= n)
            low = mid;
        else
            high = mid - 1u;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Find row i and column j */
    idx_t i = row_of_n(n);
    idx_t start = (idx_t)((__uint128_t)i * (i + 1u) / 2u);
    idx_t j = n - start;

    /* Compute binomial(i,j) multiplicatively */
    seq_t binom = 1;
    for (idx_t k = 1; k <= j; ++k) {
        binom = binom * (seq_t)(i - j + k);   /* numerator */
        binom = binom / (seq_t)k;             /* exact division */
    }

    /* Compute 3^(i-j) */
    seq_t pow3 = 1;
    for (idx_t e = 0; e < i - j; ++e)
        pow3 *= 3U;

    /* Compute 12^j */
    seq_t pow12 = 1;
    for (idx_t e = 0; e < j; ++e)
        pow12 *= 12U;

    /* Final product */
    return binom * pow3 * pow12;
}
