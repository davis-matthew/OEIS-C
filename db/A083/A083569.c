/*
 * A083569  Smallest m not occurring earlier such that m+n is prime.
 * https://oeis.org/A083569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m not occurring earlier such that m + n is
 *            prime. The sequence is a self‑inverse permutation: a(a(n)) = n
 * Range:     u8 n <= 131, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"

/* static state for memoisation */
static seq_t *cache = NULL;          /* array of computed a(n) values (0‑based) */
static size_t cache_cap = 0;         /* allocated capacity of cache */
static idx_t cache_len = 0;          /* number of terms already computed */

static bool *used = NULL;            /* used[m] = true iff value m already appeared */
static size_t used_cap = 0;          /* allocated capacity of used */

/* ensure that the used array can hold indices up to 'need-1' (0‑based) */
static void
ensure_used_capacity(size_t need)
{
    if (need <= used_cap)
        return;
    size_t new_cap = used_cap ? used_cap << 1 : 1;
    while (new_cap < need)
        new_cap <<= 1;
    bool *tmp = (bool *)realloc(used, new_cap * sizeof(bool));
    if (!tmp)                      /* allocation failure – silently ignore */
        return;
    /* initialise new portion to false */
    if (new_cap > used_cap)
        memset(tmp + used_cap, 0, (new_cap - used_cap) * sizeof(bool));
    used = tmp;
    used_cap = new_cap;
}

/* ensure that the cache array can hold indices up to 'need-1' (0‑based) */
static void
ensure_cache_capacity(idx_t need)
{
    if ((size_t)need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 1;
    while (new_cap < (size_t)need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)                      /* allocation failure – silently ignore */
        return;
    cache = tmp;
    cache_cap = new_cap;
}

/* main sequence function */
seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;         /* undefined for index 0, but return 0 */

    ensure_cache_capacity(n);

    /* compute terms up to n if not already present */
    for (idx_t k = cache_len + 1; k <= n; ++k) {
        seq_t m = 1;
        for (;;) {
            /* Indexing used array requires m to fit into size_t */
            if (m > (seq_t)SIZE_MAX)
                break;          /* safety: should not happen for allowed n range */

            size_t idx_m = (size_t)m;
            ensure_used_capacity(idx_m + 1);
            if (!used[idx_m]) {
                /* test primality of m + k */
                if (is_prime_big(m + (seq_t)k))
                    break;
            }
            ++m;
        }
        /* store result and mark it as used */
        cache[(size_t)(k - 1)] = m;
        used[(size_t)m] = true;
        ++cache_len;
    }
    return cache[(size_t)(n - 1)];
}
