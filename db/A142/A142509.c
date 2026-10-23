/*
 * A142509  Primes congruent to 3 mod 52.
 * https://oeis.org/A142509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th prime p with p % 52 == 3 (search)
 * Range:     u8 n <= 3, u16 n <= 275, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* OEIS offset is 1;  a(1) is the first term */
    if (n == 0)
        return 0;  /* undefined for n = 0, return 0 safely */

    /* Static cache of found primes (stored as uint64_t for simplicity) */
    static uint64_t *cache   = NULL;   /* array of primes congruent to 3 mod 52 */
    static size_t   sz      = 0;       /* current number of cached primes   */
    static size_t   cap     = 0;       /* allocated capacity                 */
    static uint64_t last    = 3;       /* next candidate ≡ 3 (mod 52)        */

    /* Allocate or grow the cache */
    if (cap == 0) {
        cap   = 16;
        cache = (uint64_t *)malloc(cap * sizeof(uint64_t));
        if (!cache)
            abort();                  /* memory allocation failed */
    }
    while (sz + 1 > cap) {            /* ensure space for at least one more entry */
        cap <<= 1;
        uint64_t *tmp = (uint64_t *)realloc(cache, cap * sizeof(uint64_t));
        if (!tmp)
            abort();                  /* reallocation failed */
        cache = tmp;
    }

    /* Generate primes until we have the n‑th term cached */
    while (sz < (size_t)n) {
        if (is_prime(last)) {
            cache[sz++] = last;
        }
        last += 52;   /* next number congruent to 3 modulo 52 */
    }

    return (seq_t)cache[(size_t)n - 1];   /* cast to seq_t before returning */
}
