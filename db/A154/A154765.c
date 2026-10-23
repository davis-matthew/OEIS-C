/*
 * A154765  Primes with the same number of even decimal digits and prime
 *          digits.
 * https://oeis.org/A154765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_counts, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of sequence A154765: primes whose number of even
   decimal digits equals the number of prime digits (2,3,5,7). */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/digit_counts.h"

/* Static cache of found primes */
static seq_t *cache = NULL;   /* pointer to cached primes */
static size_t cache_len = 0;  /* number of primes stored */
static size_t cache_cap = 0;  /* allocated capacity */
static seq_t cur = 2;         /* next candidate to test */

/* Grow the cache so that it can hold 'needed' elements. */
static void ensure_cache_cap(size_t needed)
{
    if (cache_cap < needed) {
        size_t new_cap = cache_cap == 0 ? 8 : cache_cap * 2;
        while (new_cap < needed) new_cap *= 2;
        seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = newp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; behavior below is irrelevant */

    while (cache_len < n) {
        /* Test current candidate for primality */
        if (is_prime_big(cur)) {
            unsigned char cnt[10];
            digit_counts(cur, cnt);
            int cnt_even = cnt[0] + cnt[2] + cnt[4] + cnt[6] + cnt[8];
            int cnt_prime = cnt[2] + cnt[3] + cnt[5] + cnt[7];
            if (cnt_even == cnt_prime) {
                ensure_cache_cap(cache_len + 1);
                cache[cache_len++] = cur;
            }
        }

        /* Advance candidate: skip even numbers > 2 */
        cur++;
        if (cur > 2 && (cur & 1) == 0) cur++;
    }

    return cache[n - 1];
}
