/*
 * A116778  Number of permutations of length n which avoid the patterns 2431,
 *          3124, 3421.
 * https://oeis.org/A116778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*

   OEIS A116778: Number of permutations of length n that avoid the patterns
   2431, 3124, 3421.

   A linear recurrence of order six holds:

        a(n) = 7 a(n–1) – 19 a(n–2) + 28 a(n–3) – 25 a(n–4)
                 + 13 a(n–5) –  3 a(n–6)          for n ≥ 7

   Initial values:

        a(1) = 1 , a(2) = 2 , a(3) = 6 , a(4) = 21 , a(5) = 73 , a(6) = 240

   The function below memoises results to allow arbitrary‑order calls.
*/

#include <stdlib.h>          /* malloc, free */

/* static array of cached values; 0‑index unused */
static seq_t *cache = NULL;          /* cache[1..max_n] holds computed a(n) */
static idx_t  cache_max = 0;         /* maximum n for which a(n) is known */
static idx_t  cache_cap = 0;         /* allocated capacity (index 0 unused) */

/* Ensure the cache has space for indices up to n and compute any missing
   values using the recurrence. */
static void
ensure_cache(idx_t n)
{
    if (n == 0) return;               /* nothing to do for n = 0 */

    /* enlarge the array if necessary */
    if (cache_cap < n + 1) {
        seq_t *new_cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!new_cache) {
            /* allocation failure – abort; this should not happen in tests */
            abort();
        }
        /* copy existing values if any */
        if (cache) {
            for (idx_t i = 1; i <= cache_max; ++i) {
                new_cache[i] = cache[i];
            }
            free(cache);
        }
        cache      = new_cache;
        cache_cap  = n + 1;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if (i == 1)      cache[i] = (seq_t)1;
        else if (i == 2) cache[i] = (seq_t)2;
        else if (i == 3) cache[i] = (seq_t)6;
        else if (i == 4) cache[i] = (seq_t)21;
        else if (i == 5) cache[i] = (seq_t)73;
        else if (i == 6) cache[i] = (seq_t)240;
        else {
            /* recurrence */
            seq_t a1 = cache[i - 1];
            seq_t a2 = cache[i - 2];
            seq_t a3 = cache[i - 3];
            seq_t a4 = cache[i - 4];
            seq_t a5 = cache[i - 5];
            seq_t a6 = cache[i - 6];

            seq_t res = a1 * (seq_t)7;
            res -= a2 * (seq_t)19;
            res += a3 * (seq_t)28;
            res -= a4 * (seq_t)25;
            res += a5 * (seq_t)13;
            res -= a6 * (seq_t)3;

            cache[i] = res;
        }
    }
    if (n > cache_max) cache_max = n;
}

seq_t
a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0 – return 0 */
    ensure_cache(n);
    return cache[n];
}
