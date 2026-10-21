/*
 * A095996  a(n) = largest divisor of n! that is coprime to n.
 * https://oeis.org/A095996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! / ∏_{p | n} p^{∑_{k≥1} ⌊n/p^k⌋}
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 22, u128 n <= 36,
 *            big any n
 * Helpers:   big_pow, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    static uint64_t *primes = NULL;
    static size_t prime_cnt = 0;
    static idx_t prime_cap = 0;

    /* generate primes up to n when needed */
    if (n > prime_cap) {
        if (primes) free(primes);
        build_primes_upto(n, &primes, &prime_cnt);
        prime_cap = n;
    }

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (n % p == 0) continue;       /* p divides n -> skip */

        /* exponent of p in n! */
        idx_t exp = 0;
        idx_t power = p;
        while (power <= n) {
            exp += n / power;
            /* guard against overflow */
            if (power > n / p) break;
            power *= p;
        }
        if (exp == 0) continue;

        seq_t term = big_pow((seq_t)p, (seq_t)exp);
        result *= term;
    }

    return result;
}
