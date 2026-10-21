/*
 * A087429  a(n) = 1 if gpf(n) < gpf(n+1), otherwise 0, where gpf = A006530
 *          (greatest prime factor).
 * https://oeis.org/A087429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if gpf(n) < gpf(n+1) else 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/largest_prime_factor.h"

seq_t a(idx_t n)
{
    uint64_t gpf_n, gpf_np1;

    /* Define gpf(1)=1; the routine returns 0 for n==1, so override. */
    if (n == 1)
        gpf_n = 1;
    else
        gpf_n = largest_prime_factor(n);

    /* n+1 is at least 2 for n>=1, safe to factor directly. */
    gpf_np1 = largest_prime_factor(n + 1);

    return (seq_t)(gpf_n < gpf_np1 ? 1 : 0);
}
