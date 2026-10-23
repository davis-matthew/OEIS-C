/*
 * A142378  Primes congruent to 27 mod 47.
 * https://oeis.org/A142378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 27 (mod 47) (OEIS A142378)
 * Range:     u8 none, u16 n <= 140, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* Undefined for n = 0, but return 0 */

    /* Initial estimate: 60*n is a generous over‑estimate for small n.
       It grows quickly enough for n up to at least 10000. */
    size_t limit = (size_t)n * 60ULL + 100ULL;
    while (1) {
        size_t prime_cnt = 0;
        uint64_t *primes = NULL;

        build_primes_upto(limit, &primes, &prime_cnt);
        if (!primes) return 0;   /* Allocation failure – unlikely */

        /* Count primes ≡ 27 (mod 47) and record the nth */
        idx_t count = 0;
        seq_t result = 0;
        for (size_t i = 0; i < prime_cnt; ++i) {
            if ((primes[i] % 47ULL) == 27ULL) {
                ++count;
                if (count == n) {
                    result = (seq_t)primes[i];
                    break;
                }
            }
        }

        /* Free the sieve array */
        free((void*)primes);

        if (result != 0) return result;

        /* Not enough found – double the sieve limit and try again */
        if (limit > SIZE_MAX / 2) {
            /* Too large to double – give up */
            return 0;
        }
        limit *= 2ULL;
    }
}
