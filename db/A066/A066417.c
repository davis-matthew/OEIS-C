/*
 * A066417  Sum of anti-divisors of n.
 * https://oeis.org/A066417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(2*n-1) + sigma(2*n+1) + sigma(n/2^k) * 2^(k+1) -
 *            6*n - 2 (n > 1) where sigma is the sum‑of‑divisors function
 *            (A000203) and k=v₂(n) is the exponent of the largest power of
 *            two dividing n. a(1) = 0
 * Range:     u8 n <= 136, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include the utility that provides sigma_of() */
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    /* a(1) is defined to be 0 */
    if (n == 1)
        return 0;

    /* compute k = v2(n) : number of trailing zero bits */
    idx_t kk = 0;
    idx_t temp = n;
    while ((temp & 1ULL) == 0) {
        ++kk;
        temp >>= 1;
    }

    /* 2*n-1 and 2*n+1 */
    idx_t two_n = n << 1;            /* 2*n, safe for idx_t up to limit */
    idx_t d1 = two_n - 1;            /* 2*n - 1 */
    idx_t d3 = two_n + 1;            /* 2*n + 1 */

    /* n/2^k */
    idx_t d2 = n >> kk;              /* shift right kk bits */

    /* Compute sigma values */
    seq_t s1 = sigma_of(d1);
    seq_t s3 = sigma_of(d3);
    seq_t s2 = sigma_of(d2);

    /* 2^(k+1) */
    seq_t pow2 = ((seq_t)1) << (kk + 1);

    /* Final formula */
    seq_t result = s1 + s3 + (s2 * pow2) - ((seq_t)6 * n) - ((seq_t)2);

    return result;
}
