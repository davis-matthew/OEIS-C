/*
 * A049418  3-i-sigma(n): sum of 3-infinitary divisors of n: if n=Product
 *          p(i)^r(i) and d=Product p(i)^s(i), each s(i) has a digit a<=b in
 *          its ternary expansion everywhere that the corresponding r(i) has a
 *          digit b, then d is a 3-i-divisor of n.
 * https://oeis.org/A049418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy, mult
 * Formula:   a(n) = Π over prime powers p^e in n of Π_{k≥0} (p^(3^k*(d_k+1))
 *            – 1)/(p^(3^k) – 1), where the base‑3 digits d_k of e are used.
 *            This implements the definition of the 3‑infinitary divisor sum
 * Range:     u8 n <= 16, u16 n <= 1008, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"   /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */
#include "../utils/seq_pow.h"     /* seq_pow(seq_t base, idx_t exp) */

/* Compute a(p^e) for a prime power using the multiplicative formula. */
static seq_t
a_prime_power(uint64_t p, uint64_t e)
{
    seq_t result = 1;
    uint64_t pow3 = 1;            /* 3^k, starting with 3^0 = 1 */

    while (e > 0) {
        uint64_t d = e % 3;       /* base‑3 digit d_k */
        e /= 3;

        if (d != 0) {
            seq_t denom = seq_pow((seq_t)p, (idx_t)pow3) - 1;
            seq_t numer = seq_pow((seq_t)p, (idx_t)(pow3 * (d + 1))) - 1;
            result *= numer / denom;   /* exact division */
        }
        pow3 *= 3;                /* next power of 3 */
    }
    return result;
}

/* Main sequence function for OEIS A049418. */
seq_t
a(idx_t n)
{
    if (n < 1) {
        /* undefined for n < 1 – the offset is 1, but we return 1 anyway. */
        return (seq_t)1;
    }

    /* Factor n into prime powers.  Arrays of size 64 are more than enough for n <= 10 000. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t total = 1;
    for (size_t i = 0; i < cnt; ++i) {
        total *= a_prime_power(primes[i], exps[i]);
    }
    return total;
}
