/*
 * A222552  Number of (n+5)X1 arrays of occupancy after each element moves up
 *          to +-5 places but not 0.
 * https://oeis.org/A222552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*a(n-1) -45*a(n-2) +84*a(n-3) -70*a(n-4) +21*a(n-5)
 *            +18*a(n-6) -200*a(n-7) +753*a(n-8) -1183*a(n-9) +631*a(n-10)
 *            +8*a(n-11) -2*a(n-12) +58*a(n-13) -32*a(n-14) -6*a(n-15)
 *            (empirical recurrence of order 15) Initial values: a(1)=456,
 *            a(2)=1668, a(3)=6220, a(4)=23341, a(5)=87752, a(6)=330274...
 * Range:     u8 none, u16 n <= 4, u32 n <= 13, u64 n <= 29, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache for computed terms. cache[i] holds a(i+1). */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* Allocated capacity of the cache */
static size_t computed = 0;    /* Number of terms currently stored */

/* Ensure the cache can hold at least 'n' terms. */
static void ensure_cache(size_t n)
{
    if (n <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < n) new_cap = n;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();   /* Allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialise the cache with the first 15 terms. */
static void init_cache(void)
{
    static const seq_t init[15] = {
        (seq_t)456ULL,          (seq_t)1668ULL,          (seq_t)6220ULL,
        (seq_t)23341ULL,        (seq_t)87752ULL,         (seq_t)330274ULL,
        (seq_t)1244135ULL,      (seq_t)4688684ULL,       (seq_t)17678240ULL,
        (seq_t)66670133ULL,     (seq_t)251457191ULL,     (seq_t)948433758ULL,
        (seq_t)3577222198ULL,   (seq_t)13491914845ULL,  (seq_t)50884787739ULL
    };
    ensure_cache(15);
    for (size_t i = 0; i < 15; ++i) cache[i] = init[i];
    computed = 15;
}

/* Compute a(n) for n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* Undefined for n=0, return 0 */
    if (!cache) init_cache();      /* Lazily initialise the cache */

    size_t idx = (size_t)(n - 1);  /* Zero‑based index in the cache */

    if (idx + 1 > computed) {
        /* Need to generate terms up to a(n). */
        ensure_cache(idx + 1);
        for (size_t i = computed; i <= idx; ++i) {
            seq_t val =  (seq_t)11  * cache[i-1]
                      - (seq_t)45  * cache[i-2]
                      + (seq_t)84  * cache[i-3]
                      - (seq_t)70  * cache[i-4]
                      + (seq_t)21  * cache[i-5]
                      + (seq_t)18  * cache[i-6]
                      - (seq_t)200 * cache[i-7]
                      + (seq_t)753 * cache[i-8]
                      - (seq_t)1183* cache[i-9]
                      + (seq_t)631 * cache[i-10]
                      + (seq_t)8   * cache[i-11]
                      - (seq_t)2   * cache[i-12]
                      + (seq_t)58  * cache[i-13]
                      - (seq_t)32  * cache[i-14]
                      - (seq_t)6   * cache[i-15];
            cache[i] = val;
        }
        computed = idx + 1;
    }
    return cache[idx];
}
