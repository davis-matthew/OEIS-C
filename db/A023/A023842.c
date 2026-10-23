/*
 * A023842  Sum of exponents in prime-power factorization of C(5n,n-5).
 * https://oeis.org/A023842
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{p prime} ( v_p((5n)!) - v_p((n-5)!) - v_p((4n+5)!) )
 *            where v_p(m!) is the exponent of prime p in m!
 * Range:     u8 n <= 680, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   simple_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/simple_primes_upto.h"

/* exponent of p in m! */
static seq_t v_p_factorial(idx_t m, uint64_t p)
{
    seq_t sum = 0;
    idx_t k = m;
    while (k) {
        k /= p;
        sum += k;
    }
    return sum;
}

/* external function required by the test harness */
seq_t a(idx_t n)
{
    if (n < 5) return (seq_t)0;

    idx_t limit = 5 * n;
    size_t prime_cnt;
    uint64_t *primes = simple_primes_upto(limit, &prime_cnt);
    if (!primes || prime_cnt == 0) return (seq_t)0;

    seq_t total = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        seq_t exp = v_p_factorial(5 * n, p)
                  - v_p_factorial(n - 5, p)
                  - v_p_factorial(4 * n + 5, p);
        total += exp;
    }

    free(primes);
    return total;
}
