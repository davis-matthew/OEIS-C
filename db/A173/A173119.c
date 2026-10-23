/*
 * A173119  Triangle T(n, k, q) = q*[n=2] + Sum_{j=0..5} q^j*binomial(n-2*j,
 *          k-j)*[n>2*j] with T(n,0) = T(n,n) = 1 for q = 3, read by rows.
 * https://oeis.org/A173119
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 158, u32 n <= 574, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A173119: a(n) is the n-th element of the triangle
   T(r,k,3) read row by row, where
   T(r,k,3) = 3*[r=2] + Σ_{j=0..5} 3^j * binomial(r-2j, k-j) * [r>2j]
   with T(r,0)=T(r,r)=1.  For 64‑bit builds values that fit are exact;
   larger values are discarded by the type width. */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/binom.h"

static inline seq_t compute_T(idx_t r, idx_t k)
{
    /* Boundary values */
    if (k == 0 || k == r)
        return (seq_t)1;

    /* Powers of 3 for j = 0..5 */
    static const seq_t pow3[6] = { (seq_t)1, (seq_t)3, (seq_t)9,
                                   (seq_t)27, (seq_t)81, (seq_t)243 };
    seq_t sum = 0;

    for (idx_t j = 0; j <= 5; ++j)
    {
        /* j contributes only while r > 2*j */
        if (r <= 2 * j)
            break;
        if (k < j)
            continue;
        idx_t a = r - 2 * j;
        idx_t b = k - j;
        if (b > a)
            continue;
        seq_t bin = binom(a, b);      /* binomial(a, b) */
        sum += pow3[j] * bin;
    }

    /* Special case r == 2, add the extra term q = 3 */
    if (r == 2)
        sum += (seq_t)3;

    return sum;
}

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = (idx_t)((sqrt((double)n * 8.0 + 1.0) - 1.0) / 2.0);

    /* Correct possible rounding errors */
    while (r * (r + 1) / 2 > n)
        --r;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    idx_t k = n - r * (r + 1) / 2;     /* column within the row */
    return compute_T(r, k);
}
