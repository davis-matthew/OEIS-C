/*
 * A062236  Sum of the levels of all nodes in all noncrossing trees with n
 *          edges.
 * https://oeis.org/A062236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=8; for n≥3 a(n) = [6*(36 n²-45 n+10)*a(n-1) - 81*(3
 *            n-5)*(3 n-1)*a(n-2)]
 *            -----------------------------------------------------------------
 *            8 n (2 n-1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation memoises values in a static array.  It uses
   128‑bit intermediate arithmetic when compiling as C (built‑in
   integral seq_t), and the generic seq_t operators otherwise
   (C++ arbitrary‑precision build).  The function is safe for any
   n≥1 and can be called in any order. */

#include <stdlib.h>

/* Static cache of already computed terms (1‑based indexing). */
static seq_t *cache   = NULL;   /* cache[1] .... cache[max_n] */
static idx_t  max_n   = 0;      /* largest n that has been computed */
static size_t cap     = 0;      /* allocated size of cache (>= max_n+1) */

/* Helper to grow the cache to hold at least n+1 elements. */
static void ensure_cache(idx_t n)
{
    if (n < cap) return;
    size_t newcap = (cap > 0 ? cap : 4);
    while (newcap <= (size_t)n) newcap <<= 1;
    cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    cap   = newcap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset starts at 1; undefined below */

    /* Initialise cache on first call. */
    if (cap == 0) {
        cap = 4;                     /* start with space for 0..3 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = 0;                /* unused to keep 1‑based indices easy */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)8;
        max_n   = 2;
    }

    ensure_cache(n);

    /* Compute missing values up to n. */
    for (idx_t i = max_n + 1; i <= n; ++i) {
#if !defined(__cplusplus)
        /* ----------------- C / builtin integral seq_t ----------------- */
        /* Use 128‑bit intermediates to avoid overflow. */
        unsigned __int128 ii = (unsigned __int128)i;
        unsigned __int128 coeff1 = 6 * ((unsigned __int128)(36*ii*ii - 45*ii + 10));
        unsigned __int128 coeff2 = 81 * ((unsigned __int128)(3*ii - 5)) *
                                  ((unsigned __int128)(3*ii - 1));
        unsigned __int128 term1 = coeff1 * (unsigned __int128)cache[i-1];
        unsigned __int128 term2 = coeff2 * (unsigned __int128)cache[i-2];
        unsigned __int128 numer = term1 - term2;
        unsigned __int128 denom = 8 * ii * (2*ii - 1);
        cache[i] = (seq_t)(numer / denom);
#else
        /* ----------------- C++ arbitrary‑precision seq_t ---------------- */
        seq_t coeff1 = (seq_t)6 *
                       ((seq_t)36 * (seq_t)i * (seq_t)i -
                        (seq_t)45 * (seq_t)i + (seq_t)10);
        seq_t coeff2 = (seq_t)81 *
                       ((seq_t)3 * (seq_t)i - (seq_t)5) *
                       ((seq_t)3 * (seq_t)i - (seq_t)1);
        seq_t term1 = coeff1 * cache[i-1];
        seq_t term2 = coeff2 * cache[i-2];
        seq_t numer = term1 - term2;
        seq_t denom = (seq_t)8 * (seq_t)i * ((seq_t)2 * (seq_t)i - (seq_t)1);
        cache[i] = numer / denom;
#endif
    }

    if (n > max_n) max_n = n;
    return cache[n];
}
