/*
 * A049131  Revert transform of (1 + 2x - x^2)/(1 + 3x + 2x^2 + x^3).
 * https://oeis.org/A049131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 11, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A049131 – Revert transform of (1 + 2x - x^2)/(1 + 3x + 2x^2 + x^3).
   Indexing:  a(1) = 1, a(2) = 1, a(3) = 2, a(4) = 4, a(5) = 10, …
   Recurrence (valid for n ≥ 6):
      8*(n-1)*n*(79*n - 186)*a(n)
           = – 2*(n-1)*(474*n^2 - 1669*n + 1510)*a(n-1)
             + (5293*n^3 - 33792*n^2 + 74125*n - 56126)*a(n-2)
             + (12719*n^3 - 97096*n^2 + 242641*n - 190562)*a(n-3)
             + (n-4)*(8927*n^2 - 44244*n + 45159)*a(n-4)
             + 23*(n-5)*(n-4)*(79*n - 107)*a(n-5);
*/

#include <stdlib.h>
#include <string.h>

/* Static cache for memoisation */
static seq_t *cache = NULL;          /* array of computed a(i) */
static idx_t  cache_size = 0;        /* number of allocated indices (size of array) */
static idx_t  max_computed = 5;      /* highest n for which a(n) is stored */

/* Ensure that cache can hold indices up to 'need' (inclusive) and
   initialise the first five terms if necessary. */
static void ensure_cache(idx_t need)
{
    if (need < 5) need = 5;               /* we always need space up to a(5)           */
    idx_t required = need + 1;             /* array indices 0..need                     */
    if (cache_size > need) return;         /* already large enough                       */

    seq_t *new_cache = (seq_t *)malloc((size_t)required * sizeof(seq_t));
    if (!new_cache) return;                /* allocation failure (should not happen)     */

    if (cache) {
        memcpy(new_cache, cache, cache_size * sizeof(seq_t));
        free(cache);
    }

    /* zero new entries */
    for (idx_t i = cache_size; i <= need; ++i)
        new_cache[i] = (seq_t)0;

    cache = new_cache;
    cache_size = (idx_t)required;

    /* initialise base values (over‑write if already present) */
    cache[0] = (seq_t)0;
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)1;
    cache[3] = (seq_t)2;
    cache[4] = (seq_t)4;
    cache[5] = (seq_t)10;
}

/* Compute a(n) with memoisation.  The function works for any order of calls. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n = 0; return 0 for safety */

    ensure_cache(n);
    if (n <= max_computed) return cache[n];

    /* Compute values from max_computed+1 up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i)
    {
        idx_t nm1 = i - 1;
        idx_t nm2 = i - 2;
        idx_t nm3 = i - 3;
        idx_t nm4 = i - 4;
        idx_t nm5 = i - 5;

        /* coefficients (non‑negative) stored in signed long long */
        long long c1 =  2LL * (long long)nm1 *
                       (474LL * (long long)i * (long long)i
                        - 1669LL * (long long)i + 1510LL);

        long long c2 =  5293LL * (long long)i * (long long)i * (long long)i
                       - 33792LL * (long long)i * (long long)i
                       + 74125LL * (long long)i
                       - 56126LL;

        long long c3 = 12719LL * (long long)i * (long long)i * (long long)i
                       - 97096LL * (long long)i * (long long)i
                       + 242641LL * (long long)i
                       - 190562LL;

        long long c4 = (long long)nm4 *
                       (8927LL * (long long)i * (long long)i
                        - 44244LL * (long long)i + 45159LL);

        long long c5 = 23LL * (long long)nm5 * (long long)nm4 *
                       (79LL * (long long)i - 107LL);

        long long d  =  8LL * (long long)nm1 *
                       (long long)i *
                       (79LL * (long long)i - 186LL);

        /* Calculate each term in seq_t (big‐int) */
        seq_t term1 = (seq_t)c1 * cache[i-1];
        seq_t term2 = (seq_t)c2 * cache[i-2];
        seq_t term3 = (seq_t)c3 * cache[i-3];
        seq_t term4 = (seq_t)c4 * cache[i-4];
        seq_t term5 = (seq_t)c5 * cache[i-5];

        /* Numerator = term2 + term3 + term4 + term5 – term1 */
        seq_t numerator = term2 + term3 + term4 + term5;
        numerator -= term1;                 /* guaranteed non‑negative */

        cache[i] = numerator / (seq_t)d;    /* exact division by recurrence */
    }

    max_computed = n;
    return cache[n];
}
