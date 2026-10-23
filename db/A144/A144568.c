/*
 * A144568  Composites with prime sum of prime factors of n and nonprime
 *          difference largest prime factor of n - smallest prime factor of n.
 * https://oeis.org/A144568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = the nth composite integer whose * (sum of prime factors
 *            with multiplicity) is prime, * and (largest prime factor –
 *            smallest prime factor) is nonprime
 * Range:     u8 n <= 27, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   is_prime_big, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"
#include "../utils/is_prime_small.h"

#define SIEVE_LIMIT 2000000

static uint32_t *spf = NULL;
static idx_t     last_checked = 3;
static seq_t   *cache       = NULL;
static idx_t     cache_size  = 0;

/* initialise smallest‑prime‑factor array up to SIEVE_LIMIT */
static void init_spf(void)
{
    if (spf != NULL)
        return;
    spf = (uint32_t *)malloc((SIEVE_LIMIT + 1) * sizeof(uint32_t));
    if (!spf)
        abort();
    for (idx_t i = 0; i <= SIEVE_LIMIT; ++i)
        spf[i] = (uint32_t)i;
    for (idx_t i = 2; (idx_t)i * i <= SIEVE_LIMIT; ++i)
        if (spf[i] == (uint32_t)i)
            for (idx_t j = i * i; j <= SIEVE_LIMIT; j += i)
                if (spf[j] == (uint32_t)j)
                    spf[j] = (uint32_t)i;
}

/* extend the cache to contain at least 'need' terms */
static void ensure_cache(idx_t need)
{
    if (cache_size >= need)
        return;

    init_spf();

    if (!cache) {
        cache = (seq_t *)malloc(need * sizeof(seq_t));
        if (!cache)
            abort();
    } else {
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!tmp)
            abort();
        cache = tmp;
    }

    for (; cache_size < need; ++cache_size) {
        for (++last_checked; ; ++last_checked) {
            if (last_checked < 4)
                continue;
            if (last_checked > SIEVE_LIMIT)
                abort();          /* pre‑computed range exhausted */

            if (spf[last_checked] == (uint32_t)last_checked)   /* prime */
                continue;

            idx_t     n     = last_checked;
            seq_t     sum   = 0;
            uint32_t  minp  = 0, maxp = 0;
            bool      first = true;

            while (n > 1) {
                uint32_t p = spf[n];
                /* count multiplicity */
                while (n % p == 0) {
                    sum += (seq_t)p;
                    n   /= p;
                }
                if (first) {
                    minp = maxp = p;
                    first = false;
                } else {
                    if (p < minp) minp = p;
                    if (p > maxp) maxp = p;
                }
            }

            uint32_t diff = maxp - minp;
            if (is_prime_big(sum) && !is_prime_small(diff)) {
                cache[cache_size] = (seq_t)last_checked;
                break;
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* undefined for offset 0, but harmless */
    ensure_cache(n);
    return cache[n - 1];
}
