/*
 * A038199  Row sums of triangle T(m,n) = number of solutions to 1 <= a(1) <
 *          a(2) < ... < a(m) <= n, where gcd(a(1), a(2), ..., a(m), n) = 1,
 *          in A020921.
 * https://oeis.org/A038199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Sum_{d|n} mu(d) * (2^{n/d}-1)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   _smallest_prime_factor, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Return the n-th term of OEIS sequence A038199.  */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/_smallest_prime_factor.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)  /* offset starts at 1, but handle 0 safely */
        return (seq_t)0;

    /* Factor n to obtain its distinct prime factors.  */
    idx_t primes[64];
    size_t pf_cnt = 0;
    idx_t temp = n;
    while (temp > 1) {
        uint64_t sp = _smallest_prime_factor((uint64_t)temp);
        /* only keep distinct primes, skip multiplicities  */
        bool already = false;
        for (size_t k = 0; k < pf_cnt; ++k)
            if (primes[k] == sp) {
                already = true;
                break;
            }
        if (!already) {
            primes[pf_cnt++] = (idx_t)sp;
        }
        /* divide out all copies of sp */
        while (temp % sp == 0)
            temp /= sp;
    }

    /* Sum over all square‑free divisors d = product of selected primes.  */
    seq_t pos = 0;  /* contributions with mu(d)=+1  */
    seq_t neg = 0;  /* contributions with mu(d)=-1 */

    size_t subsets = 1U << pf_cnt;
    for (size_t mask = 0; mask < subsets; ++mask) {
        idx_t d = 1;
        for (size_t i = 0; i < pf_cnt; ++i) {
            if (mask & (1U << i))
                d *= primes[i];
        }

        int mu = (__builtin_popcount((unsigned)mask) % 2 == 0) ? 1 : -1;
        idx_t exp = n / d;          /* n/d is an integer because d|n   */
        seq_t term = pow2(exp) - 1; /* 2^exp - 1                       */

        if (mu == 1)
            pos += term;
        else if (mu == -1)
            neg += term;
    }

    /* The final value is pos - neg; it is always non‑negative.       */
    seq_t result = pos - neg;
    return result;
}
