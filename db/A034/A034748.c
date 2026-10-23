/*
 * A034748  Dirichlet convolution of Fibonacci numbers with phi(n).
 * https://oeis.org/A034748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} F(d)*phi(n/d) (F is the Fibonacci sequence)
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 186,
 *            big any n
 * Helpers:   fib, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/fib.h"   /* seq_t fib(idx_t n);          */
#include "../utils/phi.h"   /* seq_t phi(idx_t n);          */

/* Local prime‑factorisation (simple trial division).  It is fast enough
   for n ≤ 10000 and uses only standard integer operations. */
static void factor_uint64(uint64_t n,
                          uint64_t *primes,
                          uint64_t *exps,
                          size_t *cnt)
{
    size_t c = 0;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            primes[c] = p;
            exps[c]  = e;
            ++c;
        }
    }
    if (n > 1) {               /* remaining prime factor */
        primes[c] = n;
        exps[c]   = 1;
        ++c;
    }
    *cnt = c;
}

/* Recursive generation of all divisors d of n from the factor lists
   stored in primes[0..cnt-1] and exps[0..cnt-1].  The sum is accumulated
   in *sum.  cur holds the current partial divisor. */
static void divisor_contrib(size_t idx,
                            uint64_t cur,
                            idx_t n,
                            seq_t *sum,
                            const uint64_t *primes,
                            const uint64_t *exps,
                            size_t cnt)
{
    if (idx == cnt) {
        seq_t f  = fib((idx_t)cur);
        seq_t phi_v = phi((idx_t)(n / cur));
        *sum += f * phi_v;
        return;
    }

    uint64_t p = primes[idx];
    uint64_t e = exps[idx];
    uint64_t mult = 1;
    for (uint64_t i = 0; i <= e; ++i) {
        divisor_contrib(idx + 1, cur * mult, n, sum, primes, exps, cnt);
        mult *= p;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_uint64((uint64_t)n, primes, exps, &cnt);

    seq_t sum = 0;
    divisor_contrib(0, 1, n, &sum, primes, exps, cnt);
    return sum;
}
