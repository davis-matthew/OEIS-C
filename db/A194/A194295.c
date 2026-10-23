/*
 * A194295  Triangular array: g(n,k)=number of fractional parts (i*r) in
 *          interval [(k-1)/n, k/n], for 1<=i<=n, 1<=k<=n^2, r=(1+sqrt(5))/2,
 *          the golden ratio.
 * https://oeis.org/A194295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the value g(r,p) where r = smallest integer with
 *            r*(r+1)/2 >= n p = n - (r-1)*r/2 g(r,p) = number of i (1 <= i <=
 *            r^2) such that the fractional part of (i*φ) lies in the interval
 *            [(p-1)/r , p/r), φ = (1+√5)/2
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function is written for C11 / C++17.  Only integer arithmetic is
   used for the final result; the golden‑ratio arithmetic is performed
   in long double precision, which does not influence the integer
   types seq_t and idx_t. */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* 0 is outside the defined offset, return 0. */
    if (n == 0) return 0;

    /* Find the row r.  The cumulative length of rows 1..r is r*(r+1)/2. */
    idx_t r = 0;
    while ((r * (r + 1)) / 2 < n) ++r;

    /* Position inside the row (1‑based). */
    idx_t p = n - (r - 1) * r / 2;          /* 1 <= p <= r */

    /* Allocate an array to count the occurrences of each residue 0..r-1. */
    seq_t *cnt = (seq_t *)calloc((size_t)r, sizeof(seq_t));
    if (!cnt) return 0;                      /* allocation failure */

    /* Golden ratio φ and its fractional part. */
    const long double phi      = (1.0L + sqrtl(5.0L)) / 2.0L;
    const long double frac_phi = phi - 1.0L;    /* ≈ 0.618033988749895 */

    /* Generate the fractional parts f_i = frac(i·φ) for i = 1..r². */
    long double f = frac_phi;          /* f_1 */
    const idx_t max_i = r * r;

    for (idx_t i = 1; i <= max_i; ++i) {
        /* Determine the interval index t = floor(r·f_i). */
        idx_t t = (idx_t)floorl(f * r);
        ++cnt[t];

        /* Advance to f_{i+1}.  The fractional part is increased by φ
           (the same fractional amount for each step). */
        f += frac_phi;
        if (f >= 1.0L) f -= 1.0L;
    }

    seq_t result = cnt[p - 1];
    free(cnt);
    return result;
}
