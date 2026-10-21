/*
 * A232032  Number of (n+1) X (2+1) 0..1 arrays with every element equal to
 *          some horizontal, vertical or antidiagonal neighbor, with top left
 *          element zero.
 * https://oeis.org/A232032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 5*a(n-3) - 2*a(n-4) - a(n-5) + 2*a(n-6) +
 *            2*a(n-7) Initial values: a(1)=11, a(2)=76, a(3)=521, a(4)=3588,
 *            a(5)=24719, a(6)=170270, a(7)=1172854, a(8)=8078859
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t base_vals[9] = {
    0ULL, /* dummy for 0 */
    11ULL,          /* a(1) */
    76ULL,          /* a(2) */
    521ULL,         /* a(3) */
    3588ULL,        /* a(4) */
    24719ULL,       /* a(5) */
    170270ULL,      /* a(6) */
    1172854ULL,     /* a(7) */
    8078859ULL      /* a(8) */
};

static seq_t *cache = NULL;   /* index 0 unused */
static idx_t cache_max = 0;   /* highest index computed */

static void ensure_cache(idx_t n)
{
    if (n <= cache_max) return;
    size_t new_size = (size_t)(n + 1);
    cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!cache) return;           /* out‑of‑memory: caller will crash */
    /* initialize new entries to zero (realloc may leave garbage) */
    for (idx_t i = cache_max + 1; i <= n; ++i)
        cache[i] = 0;
    /* seed base values */
    for (idx_t i = 1; i <= 8 && i <= n; ++i)
        cache[i] = base_vals[i];
    cache_max = n;
    /* compute the rest by recurrence */
    for (idx_t i = 9; i <= n; ++i) {
        seq_t term = 7 * cache[i-1];
        term += 2 * cache[i-6];
        term += 2 * cache[i-7];
        term -= 5 * cache[i-3];
        term -= 2 * cache[i-4];
        term -=    cache[i-5];
        cache[i] = term;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    ensure_cache(n);
    return cache[n];
}
