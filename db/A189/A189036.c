/*
 * A189036  a(n)= lcm(n,n')/gcd(n,n'), where n' is the arithmetic derivative
 *          of n.
 * https://oeis.org/A189036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(n,n')/gcd(n,n') where n' is the arithmetic derivative
 *            of n
 * Range:     u8 n <= 21, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* n is an unsigned 64‑bit integer.  Treat 0 as undefined but return 0. */
    if (n == 0) return 0;

    /* Factor n.  Prime list size 64 is safe for 64‑bit numbers. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(n, primes, exps, &cnt);

    seq_t nn = (seq_t)n;
    seq_t nprime = 0;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        seq_t e = (seq_t)exps[i];
        nprime += e * (nn / p);
    }

    /* gcd and lcm using the identity lcm(a,b)/gcd(a,b) = a*b / gcd(a,b)^2 */
    seq_t g = gcd(nn, nprime);
    seq_t result = (nn / g) * (nprime / g);
    return result;
}
