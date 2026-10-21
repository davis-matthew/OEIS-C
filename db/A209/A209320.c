/*
 * A209320  Number of ways to write 2n = p+q with p and q both prime, p+1 and
 *          q-1 both practical.
 * https://oeis.org/A209320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered prime pairs (p,q) with p+q = 2n such
 *            that p+1 and q-1 are both practical numbers
 * Range:     u8 n <= 884, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_practical_num, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds primes and practicality tables up to the
   required limit and counts all valid ordered pairs. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/is_prime.h"
#include "../utils/build_primes_upto.h"
#include "../utils/is_practical_num.h"

seq_t a(idx_t n)
{
    static idx_t             max_limit = 0;    /* largest limit already processed */
    static uint64_t         *primes    = NULL;  /* list of primes up to max_limit */
    static size_t            prime_cnt = 0;
    static bool              *is_practical = NULL; /* practical flag array */

    /* Determine required limit: we need to test p+1 and q-1 up to 2n+1 */
    idx_t limit = 2 * n + 1;

    /* Rebuild tables if we need a larger limit */
    if (limit > max_limit) {
        if (primes)      free(primes);
        if (is_practical) free(is_practical);

        build_primes_upto((idx_t)limit, &primes, &prime_cnt);

        is_practical = (bool *)malloc((size_t)(limit + 1) * sizeof(bool));
        if (!is_practical) return (seq_t)0; /* allocation failed: return 0 */

        /* Pre‑compute practicality for every integer up to limit */
        for (idx_t x = 0; x <= limit; ++x) {
            is_practical[x] = is_practical_num((uint64_t)x, primes, prime_cnt);
        }

        max_limit = limit;
    }

    seq_t count = (seq_t)0;
    uint64_t target = 2 * (uint64_t)n;          /* 2n */

    /* Count all ordered prime pairs (p,q) with p+q = 2n and the practicality condition. */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > target) break;                 /* further primes would make q negative */
        uint64_t q = target - p;

        if (!is_prime(q)) continue;            /* q must be prime */
        if (!is_practical[p + 1]) continue;    /* p+1 must be practical */
        if (!is_practical[q - 1]) continue;    /* q-1 must be practical */

        ++count;
    }

    return count;
}
