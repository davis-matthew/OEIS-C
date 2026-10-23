/*
 * A023529  Sum of distinct prime divisors of p(n)*p(n-1) + 1.
 * https://oeis.org/A023529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of distinct prime divisors of p(n)*p(n-1)+1
 * Range:     u8 n <= 7, u16 n <= 55, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* static state for primes up to a fixed bound */
    static bool init = false;
    static uint64_t *primes = NULL;
    static size_t prime_cnt = 0;

    if (!init) {
        /* p(10000) = 104729, so 200000 is ample */
        size_t limit = 200000;
        build_primes_upto(limit, &primes, &prime_cnt);
        init = true;
    }

    if (n < 1) return 0;

    /* primes array is zero‑indexed: primes[0]=2 = p(1) */
    uint64_t p_n   = primes[n - 1];
    uint64_t p_nm1 = (n == 1) ? 1 : primes[n - 2];

    seq_t prod = (seq_t)p_n * (seq_t)p_nm1 + 1;
    seq_t sum  = 0;
    seq_t m    = prod;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t d = primes[i];
        if ((seq_t)d * (seq_t)d > m) break;
        if (m % d == 0) {
            sum += (seq_t)d;
            while (m % d == 0) m /= d;
        }
    }
    if (m > 1) sum += (seq_t)m;   /* remaining prime factor */

    return sum;
}
