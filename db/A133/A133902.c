/*
 * A133902  a(n) = a(n-1)* d(n) if gcd(n,a(n-1))=1, otherwise a(n) =
 *          a(n-1)/gcd(n,a(n-1)). Here gcd(n,a(n-1)) is greatest common
 *          divisor, d(n) is number of divisors.
 * https://oeis.org/A133902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>0, let g=gcd(a(n-1),n); if g==1, a(n)=a(n-1)*d(n);
 *            else a(n)=a(n-1)/g; where d(n) is the number of divisors of n
 * Range:     u8 n <= 95, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_gcd.h"
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    seq_t val = 1;                     /* a(0) = 1 */
    if (n == 0)
        return val;

    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t g = seq_gcd(val, (seq_t)i);
        if (g == 1)
        {
            seq_t d = (seq_t)divisor_count_u64((uint64_t)i);
            val = val * d;
        }
        else
        {
            val = val / g;
        }
    }
    return val;
}
