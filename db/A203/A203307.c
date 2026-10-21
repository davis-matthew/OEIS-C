/*
 * A203307  a(n) = v(n+1)/(2*v(n)), where v = A203305.
 * https://oeis.org/A203307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{k=1..n} (2^k-1) * 2^(n(n+1)/2-1) (equivalently,
 *            a(n) = (1/2) * A028365(n) )
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 0)        /* out of range for the sequence; not used by tests */
        return 0;

    seq_t prod  = 1;   /* product of (2^k-1) */
    seq_t pow2k = 1;   /* 2^0 */

    for (idx_t k = 1; k <= n; ++k) {
        pow2k = pow2k * 2;      /* 2^k */
        seq_t term = pow2k - 1; /* 2^k - 1 */
        prod   = prod * term;
    }

    /* exponent for the power of two */
    seq_t nn   = (seq_t)n * ((seq_t)n + 1) / 2;
    seq_t exp  = nn - 1;          /* n(n+1)/2 - 1 */

    /* compute 2^exp and multiply */
    seq_t pow_factor = pow_uint(2ul, (idx_t)exp);
    prod = prod * pow_factor;

    return prod;
}
