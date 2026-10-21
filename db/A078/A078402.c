/*
 * A078402  Numbers k such that k^2 + 5 is prime.
 * https://oeis.org/A078402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 15, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A078402: k such that k^2 + 5 is prime.
 * All terms are multiples of 6 (k = 0, 6, 12, …).
 * This implementation generates terms on demand by testing successive
 * multiples of 6 and storing the ones whose square plus five is prime.
 * It uses the utility function is_prime_big() for primality testing.
 */

#include <stdlib.h>          /* for malloc, realloc, abort */
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;     /* array of computed terms */
static size_t cached_len = 0;   /* number of terms stored in cache      */
static size_t cached_cap = 0;   /* allocated capacity of cache          */
static idx_t next_m = 0;        /* next integer m to test (k = 6*m)    */

/* Ensure the cache has at least `need` slots. */
static void
ensure_cache(size_t need)
{
    if (need <= cached_cap)
        return;
    size_t new_cap = cached_cap ? cached_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL)
        abort();                 /* cannot allocate more memory */
    cache = tmp;
    cached_cap = new_cap;
}

/* Generate terms until the cache contains at least `n` entries. */
static void
generate_up_to(idx_t n)
{
    if ((size_t)n <= cached_len)
        return;

    ensure_cache((size_t)n);

    while (cached_len < (size_t)n)
    {
        /* candidate k = 6 * next_m */
        seq_t k = (seq_t)(6 * next_m);
        seq_t candidate = k * k + (seq_t)5;   /* compute k^2 + 5 */

        /* Test primality of candidate. */
        if (is_prime_big(candidate))
            cache[cached_len++] = k;

        /* Always increment next_m to advance the search space. */
        ++next_m;
    }
}

/* Return a(n) according to the OEIS offset (a(1) is the first listed term). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* outside the defined offset */

    generate_up_to(n);
    return cache[(size_t)n - 1];
}
