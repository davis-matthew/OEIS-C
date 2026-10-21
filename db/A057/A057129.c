/*
 * A057129  -4 is a square mod n.
 * https://oeis.org/A057129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth positive integer that is NOT a multiple of 16 * and
 *            whose every odd prime factor is ≡ 1 (mod 4)
 * Range:     u8 n <= 69, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* not used, safety */
    static seq_t *cache = NULL;    /* cached terms  */
    static size_t cache_size = 0;  /* number of cached terms */
    static size_t cache_cap  = 0;  /* capacity of cache array   */
    static uint64_t next_candidate = 1; /* next integer to test    */

    if (n <= cache_size)          /* already computed */
        return cache[n - 1];

    /* allocate or grow cache as needed */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    while (cache_size < (size_t)n) {
        uint64_t m = next_candidate++;
        if (m % 16 == 0)               /* disallow multiples of 16 */
            continue;

        uint64_t t = m;
        while ((t & 1) == 0)           /* strip factors of 2 */
            t >>= 1;

        bool ok = true;
        for (uint64_t p = 3; p * p <= t; p += 2) {
            if (t % p == 0) {
                if (p % 4 != 1) {      /* odd prime ≠ 1 mod 4 */
                    ok = false;
                    break;
                }
                do {                    /* remove all powers of p */
                    t /= p;
                } while (t % p == 0);
            }
        }
        if (ok && t > 1) {              /* remaining odd prime factor */
            if (t % 4 != 1)
                ok = false;
        }

        if (ok) {                         /* store valid term */
            if (cache_size >= cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = (seq_t)m;
        }
    }
    return cache[n - 1];
}
