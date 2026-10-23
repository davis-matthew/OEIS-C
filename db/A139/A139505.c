/*
 * A139505  Primes of the form x^2 + 25x*y + y^2 for x and y nonnegative.
 * https://oeis.org/A139505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   primes of the form x^2 + 25*x*y + y^2 for x,y ≥ 0
 * Range:     u8 none, u16 none, u32 n <= 5000, u64 n <= 5000, u128 n <= 5000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function builds a cache of such primes up to a fixed limit and
   returns the n‑th term (1‑based indexing).  It is correct for any
   seq_t type that can hold the generated primes. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

/* --- Helper: integer square root for idx_t --------------------------------- */
static idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0;
    idx_t high = n;
    idx_t mid;
    while (low <= high) {
        mid = low + (high - low) / 2;
        idx_t sq = mid * mid;
        if (sq == n) return mid;
        if (sq < n) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

/* --- Cache storage for the primes ----------------------------------------- */
static seq_t *cache = NULL;      /* array of cached primes */
static size_t  cache_size = 0;   /* number of cached primes */
static size_t  cache_cap   = 0;   /* current allocated capacity */

/* --- Comparison routine for qsort ----------------------------------------- */
static int cmp_seq_t_asc(const void *a, const void *b)
{
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

/* --- Build the cache of primes of the form x^2 + 25*x*y + y^2 --------------- */
static void ensure_cache(idx_t n)
{
    static bool built = false;
    if (built) return;  /* already built */

    /* Choose a limit that is expected to contain at least n primes.
       For n up to about 10 000 the following limit of 20 000 000 is
       sufficient.  The limit can be increased if necessary.               */
    const idx_t LIMIT = 20000000ULL;

    /* Sieve of Eratosthenes up to LIMIT.  Use a byte per number for
       simplicity; 20 000 001 bytes ~ 20 MB which is acceptable.           */
    unsigned char *isprime = (unsigned char *)malloc((size_t)(LIMIT + 1));
    unsigned char *seen    = (unsigned char *)malloc((size_t)(LIMIT + 1));
    if (!isprime || !seen) return;  /* allocation failed */

    for (size_t i = 0; i <= LIMIT; ++i) {
        isprime[i] = 1;
        seen[i]    = 0;
    }
    isprime[0] = isprime[1] = 0;

    idx_t sq = isqrt_u64(LIMIT);
    for (idx_t p = 2; p <= sq; ++p) {
        if (isprime[p]) {
            for (idx_t m = p * p; m <= LIMIT; m += p) isprime[m] = 0;
        }
    }

    /* Reserve initial capacity for the cache.  We will reallocate as needed. */
    cache_cap = 200000;
    cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache) {
        free(isprime);
        free(seen);
        return; /* allocation failed */
    }
    cache_size = 0;

    /* Enumerate all pairs (x,y) with x^2 + 25xy + y^2 ≤ LIMIT. */
    for (idx_t x = 0; ; ++x) {
        idx_t x2 = x * x;
        if (x2 > LIMIT) break;
        for (idx_t y = 0; ; ++y) {
            idx_t val = x2 + y * y + 25 * x * y;
            if (val > LIMIT) break;
            if (val >= 2 && isprime[val] && !seen[val]) {
                seen[val] = 1;
                if (cache_size == cache_cap) {
                    cache_cap *= 2;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                    if (!cache) { free(isprime); free(seen); return; }
                }
                cache[cache_size++] = (seq_t)val;
            }
        }
    }

    free(isprime);
    free(seen);

    /* Sort the cache to obtain increasing order of primes. */
    qsort(cache, cache_size, sizeof(seq_t), cmp_seq_t_asc);

    built = true;
    /* If cache_size < n the LIMIT was too small; for n ≤ 10 000
       the chosen limit suffices.  If not, the function will return
       a zero or garbage value for such indices. */
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset 0 */
    ensure_cache(n);
    return cache[n - 1];
}
