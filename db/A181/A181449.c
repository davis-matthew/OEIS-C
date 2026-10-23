/*
 * A181449  Numbers k such that 7 is the largest prime factor of k^2 - 1.
 * https://oeis.org/A181449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 13, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that 7 is the largest prime factor of k^2 - 1.
   The sequence is finite; all terms lie in the range 2 ≤ k ≤ 8749.
   We generate the list on first use and cache it for subsequent calls. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

static const idx_t LIMIT = 8749;          /* A175607(4) -- maximal k */
static seq_t *cache = NULL;              /* cached sequence values   */
static size_t  cache_len = 0;            /* number of cached terms   */

/* Check whether k satisfies the condition */
static bool is_valid(idx_t k)
{
    seq_t x = (seq_t)k * (seq_t)k - 1;
    bool has7 = false;
    const int primes[4] = {2, 3, 5, 7};

    for (int pi = 0; pi < 4; ++pi) {
        int p = primes[pi];
        while (x % p == 0) {
            x /= p;
            if (p == 7)
                has7 = true;
        }
    }
    return has7 && x == 1;
}

/* Generate all terms up to LIMIT and store them in cache */
static void init_cache(void)
{
    size_t cap = 16; /* upper bound on number of terms */
    cache = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!cache) return; /* allocation failure; silently ignore */

    for (idx_t k = 2; k <= LIMIT; ++k) {
        if (is_valid(k)) {
            /* Append k to cache */
            if (cache_len >= cap) {
                cap *= 2;
                seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                if (!tmp) return; /* allocation failure; stop adding */
                cache = tmp;
            }
            cache[cache_len++] = (seq_t)k;
        }
    }
}

/* Return the n-th term of the sequence (1‑based indexing). */
seq_t a(idx_t n)
{
    if (!cache) init_cache();
    if (n == 0 || n > (idx_t)cache_len)
        return 0; /* undefined / out of bounds */
    return cache[n - 1];
}
