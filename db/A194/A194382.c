/*
 * A194382  Numbers m such that Sum_{k=1..m} (<1/2 + k*r> - <k*r>) = 0, where
 *          r=sqrt(8) and < > denotes fractional part.
 * https://oeis.org/A194382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th positive integer m for which Sum_{k=1..m} (<1/2
 *            + k*sqrt(8)> - <k*sqrt(8)>) = 0. The implementation avoids
 *            floating point by using a 64‑bit fixed‑point representation of
 *            the irrational α = sqrt(8)-2
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

/* forward declaration required by the build system */
seq_t a(idx_t n);

/* ---------- implementation helpers ---------- */
static seq_t   *cache       = NULL;          /* stored sequence values   */
static size_t  cache_cap   = 0;              /* capacity of cache       */
static idx_t   cache_len   = 0;              /* number of values stored */

static idx_t   current_m   = 0;              /* last m processed       */
static int     diff_val    = 0;              /* running difference     */
static unsigned __int128  sum_val     = 0;    /* fractional part in Q64 */

static unsigned __int128  alpha_scaled = 0;   /* scaled irrational α   */
static const unsigned __int128 SCALE      = ((unsigned __int128)1 << 64);
static const unsigned __int128 HALF       = (SCALE >> 1);
static int     init_done     = 0;

/* resize cache when needed */
static void grow_cache(size_t new_cap)
{
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* ensure that the cache contains at least n terms   */
static void ensure_cache(idx_t n)
{
    if (cache_len >= n) return;

    if (!init_done) {
        /* initialise cache capacity */
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();

        /* compute scaled value of α = sqrt(8) - 2   */
        long double scale64 = (long double)(((unsigned __int128)1) << 64);
        long double alpha   = sqrtl(8.0L) - 2.0L;
        alpha_scaled = (unsigned __int128)(alpha * scale64 + 0.5L);

        init_done = 1;
    }

    while (cache_len < n) {
        /* next m to examine */
        ++current_m;

        /* accumulate fractional part of m*α   */
        sum_val += alpha_scaled;
        if (sum_val >= SCALE) sum_val -= SCALE;

        /* update running difference */
        diff_val += (sum_val < HALF) ? 1 : -1;

        /* record m if the balance is perfect */
        if (diff_val == 0) {
            if (cache_len == cache_cap)
                grow_cache(cache_cap ? cache_cap * 2 : 16);

            cache[cache_len++] = (seq_t)current_m;
        }
    }
}

/* exported sequence function */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n - 1];   /* offset 1 */
}
