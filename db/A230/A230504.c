/*
 * A230504  Smallest prime in r(k) = r(k-1) + gcd(k,r(k-1)) with r(1) = n.
 * https://oeis.org/A230504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime that appears in the sequence r(1) = n;
 *            r(k) = r(k-1) + gcd(k, r(k-1)) for k >= 2
 * Range:     u8 n <= 243, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    seq_t r = (seq_t)n;                 /* r(1) */
    if (is_prime_big(r))
        return r;                       /* already prime */

    for (idx_t k = 2; ; ++k) {
        seq_t step = gcd((seq_t)k, r);   /* gcd(k, r) */
        r += step;                      /* r(k) */
        if (is_prime_big(r))
            return r;                   /* first prime encountered */
    }
}
