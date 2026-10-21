/*
 * A068976  a(n) = Sum_{d | n} d/core(d) where core(x) is the smallest number
 *          such that x*core(x) is a square.
 * https://oeis.org/A068976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, mult
 * Formula:   a(n) = Sum_{d|n} gcd(d, n/d)^2. Uses the multiplicative
 *            property: a(p^e) = (p^{e+2}+p^e-2)/(p^2-1) if e is even, a(p^e)
 *            = 2*(p^{e+1}-1)/(p^2-1) if e is odd
 * Range:     u8 n <= 12, u16 n <= 1736, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a_prime_power, ppow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a_prime_power.h"
#include "../utils/ppow.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 1;          /* a(1) = 1 */

    seq_t result = 1;
    idx_t tmp = n;
    for (idx_t i = 2; i <= tmp / i; ++i) {
        if (tmp % i == 0) {
            idx_t cnt = 0;
            while (tmp % i == 0) {
                tmp /= i;
                cnt++;
            }
            result *= a_prime_power(i, cnt);
        }
    }
    if (tmp > 1) {                 /* remaining prime factor */
        result *= a_prime_power(tmp, 1);
    }
    return result;
}
