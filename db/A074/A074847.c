/*
 * A074847  Sum of 4-infinitary divisors of n: if n=Product p(i)^r(i) and
 *          d=Product p(i)^s(i), each s(i) has a digit a<=b in its 4-ary
 *          expansion everywhere that the corresponding r(i) has a digit b,
 *          then d is a 4-infinitary-divisor of n.
 * https://oeis.org/A074847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product_{ primes p^e || n } prod_{k>=0} (p^(4^k*(d_k+1))
 *            - 1)/(p^(4^k)-1) where e = sum_k d_k 4^k is the base‑4 expansion
 *            of the exponent. The function is implemented directly from the
 *            multiplicative definition
 * Range:     u8 n <= 16, u16 n <= 738, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Short trial‑division factorisation of a 64‑bit integer.
   The result is stored in the provided arrays pr[] and exp[],
   and the number of distinct prime factors is written to *cnt. */
static void factor_uint64(uint64_t n, uint64_t *pr, uint64_t *exp, size_t *cnt)
{
    size_t c = 0;
    if (n < 2) {
        *cnt = 0;
        return;
    }
    uint64_t e = 0;
    while ((n & 1ULL) == 0ULL) {
        ++e;
        n >>= 1;
    }
    if (e > 0) {
        pr[c]   = 2ULL;
        exp[c++] = e;
    }

    for (uint64_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0ULL) {
            e = 0;
            while (n % p == 0ULL) {
                ++e;
                n /= p;
            }
            pr[c]   = p;
            exp[c++] = e;
        }
    }
    if (n > 1ULL) {          /* remaining prime factor */
        pr[c]   = n;
        exp[c++] = 1ULL;
    }
    *cnt = c;
}

/* Exponentiation by squaring for the generic seq_t type. */
static inline seq_t seq_pow(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0) {
        if (exp & 1ULL)
            result *= base;
        base *= base;
        exp >>= 1ULL;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n <= 1)            /* a(1) = 1, indices <1 are not used */
        return (seq_t)1;

    uint64_t primes[32];
    uint64_t exps[32];
    size_t  pcnt;

    factor_uint64((uint64_t)n, primes, exps, &pcnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < pcnt; ++i) {
        seq_t p     = (seq_t)primes[i];
        uint64_t e  = exps[i];

        idx_t pow4k = 1;          /* 4^k, starts with 4^0 = 1 */
        while (e > 0) {
            uint64_t d = e & 3ULL;    /* d_k = e mod 4 */

            idx_t exp1 = pow4k * (d + 1);   /* exponent for numerator */

            seq_t num = seq_pow(p, exp1) - (seq_t)1;
            seq_t den = seq_pow(p, pow4k) - (seq_t)1;

            /* division is exact by the theory of the sequence */
            seq_t term = num / den;

            result *= term;

            e >>= 2;          /* e /= 4 */
            pow4k <<= 2;       /* pow4k *= 4 */
        }
    }

    return result;
}
