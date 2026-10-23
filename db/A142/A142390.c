/*
 * A142390  Primes congruent to 39 mod 47.
 * https://oeis.org/A142390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 136, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * This function computes the n‑th prime that is congruent to 39 modulo 47
 * (OEIS A142390).  It uses a simple sieve of Eratosthenes up to a
 * dynamically increased bound until enough primes are found.
 *
 * No hidden external dependencies are required besides the standard
 * C library and the types `seq_t` and `idx_t` provided by the build.
 */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* Not used by OEIS, but safe */

    uint64_t wanted = (uint64_t)n;

    /* Start with a generous upper bound.  For n <= 10000 the actual
       value is < 5 M, so this bound (20 × 47 × n) is well above the
       needed limit. */
    uint64_t limit = (n < 5) ? 500ULL : n * 47ULL * 20ULL;
    if (limit < 1000) limit = 1000;

    while (1) {
        /* Allocate array of bytes (0: prime, 1: composite) */
        uint8_t *comp = (uint8_t *)calloc(limit + 1, 1);
        if (!comp) return 0;           /* out‑of‑memory fallback */

        /* Simple sieve of Eratosthenes */
        if (limit >= 0) {
            comp[0] = comp[1] = 1;
        }

        for (uint64_t i = 2; i * i <= limit; ++i) {
            if (!comp[i]) {
                for (uint64_t j = i * i; j <= limit; j += i) {
                    comp[j] = 1;
                }
            }
        }

        /* Count primes ≡ 39 (mod 47) */
        uint64_t count = 0;
        for (uint64_t i = 2; i <= limit; ++i) {
            if (!comp[i] && (i % 47ULL) == 39ULL) {
                ++count;
                if (count == wanted) {
                    free(comp);
                    return (seq_t)i;
                }
            }
        }

        /* Not enough primes: increase the bound and try again */
        free(comp);
        limit *= 2;
    }
}
