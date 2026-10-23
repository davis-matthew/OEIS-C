/*
 * A038240  Triangle whose (i,j)-th entry is binomial(i,j)*4^(i-j)*10^j.
 * https://oeis.org/A038240
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j) * 4^(i-j) * 10^j * where i =
 *            floor((sqrt(8n+1)-1)/2) and j = n - i*(i+1)/2
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root: floor(sqrt(x)) for unsigned 64‑bit x */
static idx_t isqrt_u64(idx_t x)
{
    idx_t r = 0;
    for (int i = 63; i >= 0; --i) {
        idx_t b = ((idx_t)1) << i;
        idx_t n = r | b;
        if (n <= x / n)
            r = n;
    }
    return r;
}

/* Binomial coefficient binomial(i,j) – works for all idx_t ranges,
 * using only integer arithmetic with seq_t. */
static seq_t binom(idx_t i, idx_t j)
{
    if (j > i)
        return (seq_t)0;
    if (j > i - j)
        j = i - j;
    seq_t res = (seq_t)1;
    for (idx_t k = 1; k <= j; ++k) {
        res *= (seq_t)(i - k + 1);
        res /= (seq_t)k;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Find i such that i*(i+1)/2 <= n < (i+1)*(i+2)/2. */
    idx_t t = isqrt_u64(1 + 8 * n);
    idx_t i = (t - 1) / 2;     /* initial guess for i */

    /* Adjust in case of rounding errors. */
    while (i * (i + 1) / 2 > n)
        --i;
    while ((i + 1) * (i + 2) / 2 <= n)
        ++i;

    idx_t j = n - i * (i + 1) / 2;

    /* Compute the triangle entry. */
    seq_t val = binom(i, j);

    /* Multiply by 4^(i-j). */
    for (idx_t k = 0; k < i - j; ++k)
        val *= (seq_t)4;

    /* Multiply by 10^j. */
    for (idx_t k = 0; k < j; ++k)
        val *= (seq_t)10;

    return val;
}
