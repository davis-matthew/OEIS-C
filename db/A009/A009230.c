/*
 * A009230  a(n) = lcm(n, d(n)).
 * https://oeis.org/A009230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(n, d(n)) where d(n) is the number of divisors of n
 * Range:     u8 n <= 63, u16 n <= 4124, u32 n <= 9507, u64 n <= 9507,
 *            u128 n <= 9507, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static seq_t divisor_count(idx_t n)
{
    uint64_t nn = (uint64_t)n;
    uint64_t d  = 1;
    uint64_t p  = 2;
    while (p * p <= nn) {
        if (nn % p == 0) {
            uint64_t e = 0;
            while (nn % p == 0) {
                nn /= p;
                ++e;
            }
            d *= (e + 1);
        }
        p = (p == 2) ? 3 : p + 2;  /* skip even numbers after 2 */
    }
    if (nn > 1) { /* remaining prime factor */
        d *= 2;
    }
    return (seq_t)d;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;            /* undefined for n=0, but safe guard */
    }

    seq_t n_val    = (seq_t)n;                 /* convert index to seq_t */
    seq_t d_val    = divisor_count(n);         /* number of divisors */
    seq_t g_cmp    = gcd(n_val, d_val);        /* gcd(like seq_t) */

    /* lcm(n, d) = n / gcd(n, d) * d */
    seq_t result = (n_val / g_cmp) * d_val;
    return result;
}
