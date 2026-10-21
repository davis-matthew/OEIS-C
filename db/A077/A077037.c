/*
 * A077037  Largest prime < n^3.
 * https://oeis.org/A077037
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime < n^3 (OEIS A077037)
 * Range:     u8 n <= 6, u16 n <= 35, u32 n <= 79, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Static cache for previously computed values */
static seq_t *prime_cache = NULL;
static size_t cache_cap = 0;

/* Ensure the cache can hold values up to index n */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_cap)
        return;

    seq_t *tmp = (seq_t *)realloc(prime_cache, needed * sizeof(seq_t));
    if (!tmp)
        exit(1);
    prime_cache = tmp;

    /* Zero the newly added portion */
    if (cache_cap < needed)
        memset((void *)(prime_cache + cache_cap), 0,
               (needed - cache_cap) * sizeof(seq_t));

    cache_cap = needed;
}

/* Return the largest prime < n^3 */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                 /* undefined for n < 2, harmless */

    ensure_cache(n);
    if (prime_cache[n] != 0)
        return prime_cache[n];

    /* Compute n^3 in seq_t arithmetic */
    seq_t nn = (seq_t)n;
    seq_t cube = nn * nn * nn;    /* n^3 */

    seq_t candidate = cube - 1;   /* start just below n^3 */

    /* Fast descent: skip even numbers (except 2) */
    if (candidate >= 3 && (candidate & 1) == 0)
        candidate--;              /* make it odd */

    while (candidate >= 2) {
        if (is_prime_big(candidate)) {
            prime_cache[n] = candidate;
            return candidate;
        }
        if (candidate == 3) {      /* next odd would be 1 */
            candidate = 2;
            continue;
        }
        candidate -= 2;            /* skip the even number below */
    }

    /* Fallback (should never happen) */
    prime_cache[n] = 2;
    return 2;
}
