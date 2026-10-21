/*
 * A234241  Number of (n+1) X (1+1) 0..7 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 2
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) + 181*a(n-2) - 1022*a(n-3) - 7657*a(n-4) +
 *            21504*a(n-5) + 54144*a(n-6) - 96768*a(n-7) - 80640*a(n-8) for n
 *            > 8, with initial values a(1)=630, a(2)=6706, a(3)=72616,
 *            a(4)=790718, a(5)=8612920, a(6)=93934380, a(7)=1023822944,
 *            a(8)=11170450750
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 16, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Cache for previously computed terms: indices 0..cache_len, where
       index 0 is unused.  cache_len is the largest index stored.   */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;          /* number of terms cached (0 means none) */

    /* Initial terms that are known a priori.  They are stored as 64‑bit
       unsigned constants; conversion to seq_t is done on assignment. */
    static const uint64_t init_vals[8] = {
        630ULL,          /* a(1) */
        6706ULL,         /* a(2) */
        72616ULL,        /* a(3) */
        790718ULL,       /* a(4) */
        8612920ULL,      /* a(5) */
        93934380ULL,     /* a(6) */
        1023822944ULL,   /* a(7) */
        11170450750ULL   /* a(8) */
    };

    /* --- Initialization -------------------------------------------------- */
    if (cache_len == 0) {
        cache_len = 8;                     /* we will store indices 0..8 */
        cache = (seq_t *)malloc((cache_len + 1) * sizeof(seq_t));
        if (!cache) abort();               /* allocation failure */

        /* cache[0] is unused; store initial terms in cache[1..8] */
        for (idx_t i = 1; i <= 8; ++i)
            cache[i] = (seq_t)init_vals[i - 1];
    }

    /* Return cached value if requested index is already stored. */
    if (n <= cache_len)
        return cache[n];

    /* --- Extend cache to hold up to index n -------------------------------- */
    idx_t new_len = n;                       /* new maximum index needed */
    seq_t *new_cache = (seq_t *)malloc((new_len + 1) * sizeof(seq_t));
    if (!new_cache) abort();

    /* Copy existing cached values into the new array. */
    for (idx_t i = 0; i <= cache_len; ++i)
        new_cache[i] = cache[i];

    /* Compute missing terms using the recurrence. */
    for (idx_t i = cache_len + 1; i <= new_len; ++i) {
        seq_t val = 0;
        val += new_cache[i - 1] * 7;
        val += new_cache[i - 2] * 181;
        val -= new_cache[i - 3] * 1022;
        val -= new_cache[i - 4] * 7657;
        val += new_cache[i - 5] * 21504;
        val += new_cache[i - 6] * 54144;
        val -= new_cache[i - 7] * 96768;
        val -= new_cache[i - 8] * 80640;
        new_cache[i] = val;
    }

    /* Free the old cache and replace it with the enlarged one. */
    free(cache);
    cache = new_cache;
    cache_len = new_len;

    return cache[n];
}
