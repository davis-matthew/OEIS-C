/*
 * A080007  Number of permutations satisfying -k<=p(i)-i<=r and p(i)-i not in
 *          I, i=1..n, with k=2, r=3, I={0,1}.
 * https://oeis.org/A080007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A080007
   Recurrence: a(n) = a(n-2)+2*a(n-3)+2*a(n-4)+4*a(n-5)
                      -2*a(n-7)-a(n-8)-a(n-10)
   Initial values (n = 0..9):
      1,0,0,1,2,4,4,8,19,32
*/
#include <stdlib.h>

static seq_t *cache      = NULL;   /* array of computed terms      */
static size_t cache_size = 0;      /* number of terms stored (next free index) */
static size_t cache_cap  = 0;      /* allocated capacity           */

/* ensure that 'cache' can hold at least 'need' elements */
static void ensure_cache(size_t need)
{
    if (need <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap < need)
        new_cap <<= 1;

    seq_t *newp = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!newp) exit(EXIT_FAILURE);       /* abort on allocation failure */

    if (cache) {
        for (size_t i = 0; i < cache_size; ++i)
            newp[i] = cache[i];         /* copy existing terms */
        free(cache);                    /* free old storage */
    }
    cache     = newp;
    cache_cap = new_cap;
}

/* initialise the base cases (a(0)…a(9)) */
static void init_cache(void)
{
    if (cache) return;                     /* already initialised */
    ensure_cache(10);
    static const seq_t init_vals[10] = {1,0,0,1,2,4,4,8,19,32};
    for (size_t i = 0; i < 10; ++i)
        cache[i] = init_vals[i];
    cache_size = 10;                      /* indices 0..9 are valid */
}

seq_t a(idx_t n)
{
    init_cache();                         /* guarantee base values exist */

    if ((size_t)n < cache_size)           /* already computed */
        return cache[(size_t)n];

    /* we need to compute terms up to n (inclusive) */
    ensure_cache((size_t)n + 1);

    for (idx_t i = cache_size; i <= n; ++i) {
        if (i < 10) {                     /* safety – should not happen */
            continue;
        }
        seq_t val = cache[i - 2];
        val += cache[i - 3] << 1;          /* 2*a(i-3) */
        val += cache[i - 4] << 1;          /* 2*a(i-4) */
        val += cache[i - 5] << 2;          /* 4*a(i-5) */
        val -= cache[i - 7] << 1;          /* -2*a(i-7) */
        val -= cache[i - 8];               /* -a(i-8)  */
        val -= cache[i -10];               /* -a(i-10) */
        cache[i] = val;
    }
    cache_size = (size_t)n + 1;             /* all terms up to n are ready */

    return cache[(size_t)n];
}
