/*
 * A158303  Triangle read by rows, A007318 * (A158300 * 0^(n-k)).
 * https://oeis.org/A158303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binomial(r, k) * A158300(k), where r =
 *            floor((sqrt(8*n+1)-1)/2), k = n - r(r+1)/2, A158300(0)=1,
 *            A158300(k>0)=2^(2*floor((k+1)/2)-1)
 * Range:     u8 n <= 30, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   A158300
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/A158300.h"

/* ---- helper: floor integer square root of a 64‑bit value ---- */
static idx_t floor_sqrt_u64(idx_t x)
{
    /* safe for all 64‑bit x on the targeted ranges */
    return (idx_t)floor(sqrt((long double)x));
}

/* ---- helper: great‑common‑divisor for seq_t ---- */
static seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* ---- helper: binomial coefficient (n choose k) with overflow control ---- */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n)                 return 0;
    if (k == 0 || k == n)      return 1;
    if (k > n - k)             k = n - k;

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = n - k + i;  /* numerator part */
        idx_t   den = i;        /* denominator part */

        /* Reduce num and den by their gcd first */
        seq_t g1 = seq_gcd(num, (seq_t)den);
        num /= g1;
        den /= (idx_t)g1;

        /* Reduce res and remaining denominator */
        seq_t g2 = seq_gcd(res, (seq_t)den);
        res /= g2;
        den /= (idx_t)g2;

        res = res * num;
        if (den != 1)          /* safety: den should now be 1 */
            res = res / (seq_t)den;
    }
    return res;
}

/* ---- main sequence function ---- */
seq_t a(idx_t n)
{
    /* find row r such that n lies in the triangle */
    idx_t r = (floor_sqrt_u64(8 * n + 1) - 1) >> 1;
    idx_t tri = r * (r + 1) / 2;          /* first index of row r */
    idx_t k   = n - tri;                  /* column within the row */

    seq_t val = binom(r, k);              /* binomial coefficient */
    val = val * A158300(k);               /* multiply by diagonal entry */
    return val;
}
