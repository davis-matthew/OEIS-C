/*
 * A056939  Array read by antidiagonals: number of antichains (or order
 *          ideals) in the poset 3*m*n or plane partitions with rows <= m,
 *          columns <= n and entries <= 3.
 * https://oeis.org/A056939
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl, nice
 * Formula:   A056939 is read by antidiagonals of the infinite array *
 *            produced by ∏_{k=0..2} binom(i+j+k, j+k)/binom(i+k, k). * For a
 *            given index n, let s = floor((sqrt(1+8n)-1)/2) * (i.e. the
 *            largest integer with s(s+1)/2 ≤ n), and k = n - s(s+1)/2. * Then
 *            the term a(n) equals A(i,j) with i = k, j = s-k
 * Range:     u8 n <= 22, u16 n <= 48, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note:  binomial() and gcd() are provided by the utils library. */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/gcd.h"

/* Find the diagonal number s such that s*(s+1)/2 ≤ n < (s+1)*(s+2)/2.  */
static idx_t find_diagonal(idx_t n)
{
    idx_t low = 0, high = 1;
    /* exponential search to find an upper bound */
    while ((unsigned __int128)high * (high + 1) / 2 <= n)
    {
        high <<= 1;
        if (high == 0)   /* overflow guard – not needed for n≤10000 */
            break;
    }
    low = high >> 1;
    /* binary search between low and high */
    while (low < high)
    {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val <= n)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;
}

/* Multiply res by f after cancelling any common factors with the three
 * denominators den0, den1, den2.  The denominators are updated in place. */
static void mul_reduce(seq_t *res, seq_t f,
                       seq_t *den0, seq_t *den1, seq_t *den2)
{
    seq_t g;

    g = gcd(f, *den0);
    if (g > 1) { f /= g; *den0 /= g; }

    g = gcd(f, *den1);
    if (g > 1) { f /= g; *den1 /= g; }

    g = gcd(f, *den2);
    if (g > 1) { f /= g; *den2 /= g; }

    *res *= f;
}

seq_t a(idx_t n)
{
    /* Find the diagonal and the coordinates (i,j) on it. */
    idx_t s   = find_diagonal(n);
    idx_t prev = (idx_t)((unsigned __int128)s * (s + 1) / 2);
    idx_t k   = n - prev;
    idx_t i   = k;          /* row index   */
    idx_t j   = s - k;      /* column index */

    /* Denominator factors coming from the product formula. */
    seq_t den0 = (seq_t)(i + 1); /* (i+1) */
    seq_t den1 = (seq_t)(i + 1); /* (i+1) again */
    seq_t den2 = (seq_t)(i + 2); /* (i+2)    */

    seq_t result = 1;

    /* Factor 2 from the simplified formula. */
    mul_reduce(&result, (seq_t)2, &den0, &den1, &den2);

    /* The three binomial factors from the product. */
    seq_t b1 = binomial(i + j,     j);
    seq_t b2 = binomial(i + j + 1, j + 1);
    seq_t b3 = binomial(i + j + 2, j + 2);

    mul_reduce(&result, b1, &den0, &den1, &den2);
    mul_reduce(&result, b2, &den0, &den1, &den2);
    mul_reduce(&result, b3, &den0, &den1, &den2);

    /* Any remaining denominator factors must divide the result exactly. */
    if (den0 > 1) result /= den0;
    if (den1 > 1) result /= den1;
    if (den2 > 1) result /= den2;

    return result;
}
