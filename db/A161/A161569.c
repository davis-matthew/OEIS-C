/*
 * A161569  Sum of first n nonprimes minus their indices.
 * https://oeis.org/A161569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A051349(n) - n*(n+1)/2 Also a(n) = a(n-1) + A018252(n) -
 *            n, with a(1)=0
 * Range:     u8 n <= 30, u16 n <= 755, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>


/* Simple primality test by trial division. */
static bool is_prime_uint64(uint64_t m)
{
    if (m < 2) return false;
    if ((m & 1ULL) == 0) return m == 2;
    for (uint64_t d = 3; d * d <= m; d += 2)
        if (m % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    /* Cache stores pre‑computed values a[1..computed_up_to] */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t computed_up_to = 0;

    /* Next integer to test for the next non‑prime. */
    static uint64_t next_candidate = 2;

    if (n == 0)
        return 0;                      /* undefined but safe */

    /* Initialise cache on first call. */
    if (cache == NULL) {
        cache_cap = n + 1;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache) abort();           /* allocation failure */

        cache[1] = (seq_t)0;           /* a(1)=0 */
        computed_up_to = 1;
        next_candidate = 2;
    }
    /* Grow cache if needed. */
    else if (cache_cap <= n) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Return cached result if already known. */
    if (n <= computed_up_to)
        return cache[n];

    /* Compute missing terms up to n. */
    for (idx_t idx = computed_up_to + 1; idx <= n; ++idx) {
        uint64_t vp;
        /* Find next non‑prime starting from next_candidate. */
        while (true) {
            vp = next_candidate++;
            if (!is_prime_uint64(vp))
                break;
        }
        /* a(idx) = a(idx-1) + vp - idx */
        cache[idx] = cache[idx - 1] + (seq_t)vp - (seq_t)idx;
    }
    computed_up_to = n;
    return cache[n];
}
