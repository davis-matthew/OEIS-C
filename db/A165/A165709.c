/*
 * A165709  Number of reduced words of length n in Coxeter group on 49
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^9 = I.
 * https://oeis.org/A165709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 47*(a(n-1)+a(n-2)+…+a(n-8)) - 1128*a(n-9) + P(n) where
 *            P(0)=1, P(1..8)=2, P(9)=1, and P(n)=0 for n>9
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* #include <stdbool.h> unnecessary for this function, but allowed if needed. */
#include <stdlib.h>

static const unsigned char PNUM[10] = {1, 2, 2, 2, 2, 2, 2, 2, 2, 1};

seq_t a(idx_t n)
{
    /* Static storage for memoisation */
    static seq_t *cache      = NULL;
    static idx_t cache_size  = 0;      /* allocated entries count   */
    static idx_t computed_up = 0;      /* last index computed      */
    static int   init_done   = 0;      /* init flag                 */

    /* Extend the cache if needed */
    if (cache_size <= n) {
        /* realloc returns void*, cast explicitly for C++ */
        void *tmp = realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) abort();                  /* allocation failure */
        cache      = (seq_t *)tmp;
        cache_size = n + 1;
    }

    /* Initialise the first term once */
    if (!init_done) {
        cache[0] = (seq_t)PNUM[0];
        init_done = 1;
    }

    /* Compute all missing terms up to n */
    if (n > computed_up) {
        for (idx_t k = computed_up + 1; k <= n; ++k) {
            seq_t val = 0;
            /* 47 * a(k-1) … a(k-8) term */
            for (idx_t i = 1; i <= 8; ++i) {
                if (k >= i)
                    val += ((seq_t)47) * cache[k - i];
                else
                    break;
            }
            /* -1128 * a(k-9) term */
            if (k >= 9)
                val -= ((seq_t)1128) * cache[k - 9];

            /* add P(k) if k <= 9 */
            if (k <= 9)
                val += (seq_t)PNUM[(size_t)k];

            cache[k] = val;
        }
        computed_up = n;
    }

    return cache[n];
}
