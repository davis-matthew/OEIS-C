/*
 * A092519  Smallest prime a(n) such that a(n)+6n, a(n)+12n and a(n)+18n are
 *          also primes.
 * https://oeis.org/A092519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime p such that p+6n, p+12n, and p+18n are all
 *            prime
 * Range:     u8 n <= 12, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/next_prime.h"
#include "../utils/is_prime_big.h"

static inline bool candidate_ok(idx_t n, uint64_t p)
{
    seq_t six  = (seq_t)6  * (seq_t)n;
    seq_t twelve = (seq_t)12 * (seq_t)n;
    seq_t eighteen = (seq_t)18 * (seq_t)n;
    seq_t cand6  = (seq_t)p + six;
    seq_t cand12 = (seq_t)p + twelve;
    seq_t cand18 = (seq_t)p + eighteen;
    return is_prime_big(cand6) && is_prime_big(cand12) && is_prime_big(cand18);
}

seq_t a(idx_t n)
{
    uint64_t p = 2;          /* first prime */
    while (true) {
        if (candidate_ok(n, p))
            return (seq_t)p; /* found the smallest qualifying prime */
        p = next_prime(p);   /* advance to the next prime */
    }
}
