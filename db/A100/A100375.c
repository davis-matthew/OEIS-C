/*
 * A100375  a(n) is the n-th consecutive prime difference divided by the
 *          largest power of 2 which divides it.
 * https://oeis.org/A100375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (p_{n+1} - p_n) / 2^{v2(p_{n+1} - p_n)}
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_uint64.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                  /* below offset, any value */

/* Prime generation state */
    static uint64_t *primes = NULL;          /* array of primes          */
    static size_t primes_count = 0;          /* number of primes stored  */
    static size_t primes_capacity = 0;       /* allocated capacity       */
    static uint64_t next_candidate = 5;      /* odd number to try next   */

    /* initialise on first call */
    if (primes_count == 0) {
        primes_capacity = 16;
        primes = (uint64_t *)malloc(primes_capacity * sizeof(uint64_t));
        if (!primes) return (seq_t)0;          /* allocation failure       */
        primes[0] = 2;
        primes[1] = 3;
        primes_count = 2;
    }

    /* ensure we have primes[n] and primes[n-1] (0‑based) */
    while (primes_count <= (size_t)n) {
        uint64_t candidate = next_candidate;
        while (!is_prime_uint64(candidate))
            candidate += 2;

        if (primes_count >= primes_capacity) {
            size_t new_cap = primes_capacity ? primes_capacity * 2 : 4;
            uint64_t *tmp = (uint64_t *)realloc(primes,
                                                new_cap * sizeof(uint64_t));
            if (!tmp) return (seq_t)0;          /* allocation failure       */
            primes = tmp;
            primes_capacity = new_cap;
        }
        primes[primes_count++] = candidate;
        next_candidate = candidate + 2;
    }

    /* compute the odd part of the difference */
    seq_t diff = (seq_t)primes[(size_t)n] - (seq_t)primes[(size_t)n - 1];
    const seq_t ONE = (seq_t)1;
    while ((diff & ONE) == 0)
        diff >>= 1;

    return diff;
}
