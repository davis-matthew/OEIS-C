/*
 * A095691  Multiplicative with a(p^e) = A000720(e)+1.
 * https://oeis.org/A095691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn, easy
 * Formula:   a(n) = product over prime powers dividing n of (π(e)+1), where
 *            π(e) is the number of primes ≤ e. (Multiplicative,
 *            a(p^e)=A000720(e)+1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* Small table of primes up to 61 to compute π(e) for e ≤ 64. */
static const uint8_t primes_upto_64[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47, 53, 59, 61
};

/* Count the primes ≤ e (e ≤ 64). */
static uint64_t prime_count_upto(uint64_t e)
{
    uint64_t c = 0;
    for (size_t i = 0; i < sizeof(primes_upto_64) / sizeof(primes_upto_64[0]); ++i) {
        if (primes_upto_64[i] > e) break;
        ++c;
    }
    return c;
}

seq_t a(idx_t n)
{
    seq_t result = 1;
    uint64_t val = (uint64_t)n;

    /* Factor n into primes.  The maximum number of distinct prime factors of a
       64‑bit integer is 64, so the buffers are safely large. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(val, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e   = exps[i];
        uint64_t pc  = prime_count_upto(e);  /* π(e) */
        result *= (seq_t)(pc + 1);           /* a(p^e) */
    }
    return result;
}
