/*
 * A183098  a(1) = 0, a(n) = sum of divisors d of n such that if d =
 *          Product_{i} (p_i^e_i) then not all e_i are > 1.
 * https://oeis.org/A183098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = σ(n) – Π p (1 + Σ_{k=2}^{e_p} p^k) (sum of divisors minus
 *            sum of powerful divisors, including 1)
 * Range:     u8 n <= 119, u16 n <= 9771, u32 n <= 9771, u64 n <= 9771,
 *            u128 n <= 9771, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 0;  otherwise a(n) = σ(n) – Π p (1 + Σ_{k=2}^{e_p} p^k)   */

#include <stdint.h>
#include <stddef.h>

/* Small primes up to 100 – enough for trial division of n ≤ 10⁴ */
static const uint64_t _PRIMES[] = {
    2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,
    53,59,61,67,71,73,79,83,89,97
};

seq_t a(idx_t n)
{
    if (n == 1) {
        return 0;
    }

    seq_t sigma = 1;      /* product of (1 + p + … + p^e)  – sum of all divisors */
    seq_t prodPower = 1;  /* product of (1 + Σ_{k=2}^{e} p^k)   – sum of powerful divisors, including 1 */

    uint64_t m = (uint64_t)n;
    size_t pi = 0;

    while (pi < sizeof(_PRIMES)/sizeof(_PRIMES[0]) && _PRIMES[pi] * _PRIMES[pi] <= m) {
        uint64_t p = _PRIMES[pi];
        if (m % p == 0) {
            uint64_t exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
            }

            /* compute (1 + p + … + p^exp)  for sigma */
            seq_t sumAll = 1;
            seq_t pow = 1;
            for (uint64_t k = 1; k <= exp; ++k) {
                pow *= (seq_t)p;
                sumAll += pow;
            }
            sigma *= sumAll;

            /* compute (1 + Σ_{k=2}^{exp} p^k)  for prodPower */
            seq_t factorPower = 1;          /* the “0'' exponent term (1) */
            if (exp >= 2) {
                seq_t pow2 = (seq_t)p * (seq_t)p;   /* p^2 */
                seq_t sumPow = pow2;
                for (uint64_t k = 3; k <= exp; ++k) {
                    pow2 *= (seq_t)p;
                    sumPow += pow2;
                }
                factorPower += sumPow;      /* add the Σ_{k=2}^{exp} p^k part */
            }
            prodPower *= factorPower;
        }
        pi++;
    }

    /* Remaining prime factor (if any) has exponent 1 */
    if (m > 1) {
        uint64_t p = m;                      /* exp = 1 */
        seq_t sumAll = 1 + (seq_t)p;
        sigma *= sumAll;
        /* factorPower remains 1 because exp < 2 */
    }

    return sigma - prodPower;
}
