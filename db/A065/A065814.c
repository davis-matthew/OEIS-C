/*
 * A065814  a(n) = tau(n)^2 - tau(n^2), where tau(n) = A000005(n).
 * https://oeis.org/A065814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = tau(n)^2 - tau(n^2)
 * Range:     u8 n <= 239, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute a(n) = tau(n)^2 - tau(n^2), where tau(n) is the number of
   divisors of n.  This implementation factors n by trial division,
   which is adequate for n up to about 10,000 (the typical call range). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for 0, return 0 */

    uint64_t nn   = n;                     /* working copy of n */
    uint64_t tau  = 1;                     /* tau(n)   */
    uint64_t tau_nsq = 1;                  /* tau(n^2) */

    for (uint64_t p = 2; p * p <= nn; ++p)
    {
        if (nn % p == 0)
        {
            int exp = 0;
            while (nn % p == 0)
            {
                nn /= p;
                ++exp;
            }
            tau       *= (uint64_t)(exp + 1);
            tau_nsq   *= (uint64_t)(2 * exp + 1);
        }
    }

    if (nn > 1)                 /* remaining prime factor */
    {
        tau       *= 2;
        tau_nsq   *= 3;
    }

    seq_t tau_s  = (seq_t)tau;
    seq_t tau_ns = (seq_t)tau_nsq;
    return tau_s * tau_s - tau_ns;
}
