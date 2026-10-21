/*
 * A065982  a(n) = (n+1)*binomial(2*n,n) - 2^(2*n-1).
 * https://oeis.org/A065982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)*binomial(2*n,n) - 2^(2*n-1) (offset 1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t central_binomial(idx_t n)
{
    /* Compute C(2*n, n) multiplicatively to avoid overflow.
       The result fits in seq_t as per build configuration. */
    seq_t res = 1;
    for (idx_t i = 1; i <= n; ++i) {
        /* res = res * (n + i) / i;  the division is exact. */
        res = res * (n + i);
        res = res / i;
    }
    return res;
}

static seq_t pow2_shift(idx_t exp)
{
    /* Compute 2^exp by repeated left shifts.  The number of iterations
       is at most 2*n-1 (≈ 20000 for n=10000), which is negligible. */
    seq_t v = 1;
    while (exp--) {
        v <<= 1;
    }
    return v;
}

seq_t a(idx_t n)
{
    /* n >= 1 as per sequences' offset.  No special handling for n==0 is
       required because calls will not be made for that index. */
    seq_t cb = central_binomial(n);          /* C(2n, n) */
    seq_t term1 = (seq_t)(n + 1) * cb;       /* (n+1) * C(2n, n) */
    seq_t term2 = pow2_shift((idx_t)(2 * n - 1));  /* 2^(2n-1) */
    return term1 - term2;
}
