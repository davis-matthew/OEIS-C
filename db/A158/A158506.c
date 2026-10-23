/*
 * A158506  Determinants of n-times-n matrices M of the form M[i,j] = 2^(i*j).
 * https://oeis.org/A158506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^((n*(n+1)*(n+2))/6) * ∏_{i=1}^{n-1} (2^i-1)^(n-i)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_mul.h"

/* fast exponentiation for seq_t values, using only seq_mul */
static seq_t seq_pow_uint(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1)
            result = seq_mul(result, base);
        exp >>= 1;
        if (exp)
            base = seq_mul(base, base);
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* safety for unexpected calls – offset is 1 */
        return (seq_t)0;

    /* exponent for 2^(n(n+1)(n+2)/6) */
    idx_t exp1 = n * (n + 1) * (n + 2) / 6;
    seq_t res = seq_pow_uint((seq_t)2, exp1);

    for (idx_t i = 1; i < n; ++i) {
        seq_t two_pow = seq_pow_uint((seq_t)2, i);        /* 2^i */
        seq_t base = two_pow - (seq_t)1;                  /* 2^i – 1 */
        idx_t pwr = n - i;
        seq_t term = seq_pow_uint(base, pwr);             /* (2^i-1)^(n-i) */
        res = seq_mul(res, term);
    }
    return res;
}
