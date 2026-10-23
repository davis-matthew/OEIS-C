/*
 * A117584  Generalized Pellian triangle.
 * https://oeis.org/A117584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = element at the n-th position in the * antidiagonal
 *            enumeration of the generalized Pellian array. * The array is
 *            defined by * T(n,k) = P(k) + (n-1)*P(k-1), * where P(m) is the
 *            m-th Pell number with P(0)=0, P(1)=1, * and the anti‑diagonals
 *            are listed row‑by‑row
 * Range:     u8 n <= 35, u16 n <= 104, u32 n <= 376, u64 n <= 1377,
 *            u128 n <= 5050, big any n
 * Helpers:   pell_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/pell_k.h"

/* Compute integer floor(sqrt(x)) for 0 <= x <= 2^67
   using long double arithmetic.  The result fits in uint64_t. */
static idx_t isqrt_u128(__uint128_t x)
{
    long double d = sqrtl((long double)x);
    idx_t r = (idx_t)d;                    /* initial truncation */
    /* Correct possible rounding errors */
    while ((__uint128_t)(r + 1) * (r + 1) <= x) ++r;
    while ((__uint128_t)r * r > x) --r;
    return r;
}

/* Compute T(n,k) = P(k) + (n-1)*P(k-1).  n,k >= 1. */
static seq_t compute_T(idx_t n, idx_t k)
{
    seq_t pk = pell_k(k);
    seq_t res = pk;
    if (k > 1) {
        seq_t pk1 = pell_k(k - 1);
        res += (seq_t)(n - 1) * pk1;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n=0, return 0 gracefully */

    /* Find diagonal d such that offset(d) <= n < offset(d+1) */
    idx_t m = n - 1;             /* n-1 >= 0 */
    __uint128_t val = (__uint128_t)8 * m + 1;   /* 8*(n-1)+1, fits in 128 bits */
    idx_t s = isqrt_u128(val);     /* s = floor(sqrt(8*(n-1)+1)) */

    idx_t t = (s + 1) / 2;         /* t = floor((s+1)/2) */
    idx_t d = t + 1;               /* diagonal index */

    /* offset(d) = 1 + (d-2)(d-1)/2, computed in 128-bit to avoid overflow */
    idx_t offset_d = 1 + (idx_t)(((__uint128_t)(d - 2) * (d - 1)) >> 1);

    idx_t pos = n - offset_d;          /* 0‑based position within diagonal */
    idx_t n_val = d - 1 - pos;         /* row number in the array */
    idx_t k_val = pos + 1;             /* column number in the array */

    return compute_T(n_val, k_val);
}
