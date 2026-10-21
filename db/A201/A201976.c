/*
 * A201976  Number of n X 3 0..3 arrays with rows and columns
 *          lexicographically nondecreasing read forwards and nonincreasing
 *          read backwards.
 * https://oeis.org/A201976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 34, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A201976
    Formula: a(n) = 7*a(n-1) - 21*a(n-2) + 35*a(n-3) - 35*a(n-4)
              + 21*a(n-5) - 7*a(n-6) + a(n-7)   for n>7
    Initial terms (n=1..7):
        4, 72, 1372, 9260, 38112, 117744, 301612
*/

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;      /* cache of computed values */
static idx_t cache_len = 0;      /* number of elements currently cached */

/* Ensure that cache contains values up to index n (1‑based) */
static void ensure_cache(idx_t n)
{
    if (cache_len >= n)
        return;                          /* already available */

    /* grow the cache */
    seq_t *new_cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
    if (!new_cache)                      /* out‑of‑memory */
        abort();
    cache = new_cache;

    /* fill new entries, starting after the already cached ones */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        switch (i) {
            case 1: cache[0] = (seq_t)4;          break;
            case 2: cache[1] = (seq_t)72;         break;
            case 3: cache[2] = (seq_t)1372;       break;
            case 4: cache[3] = (seq_t)9260;       break;
            case 5: cache[4] = (seq_t)38112;      break;
            case 6: cache[5] = (seq_t)117744;     break;
            case 7: cache[6] = (seq_t)301612;     break;
            default: {
                /*  a(i) = 7*a(i-1) - 21*a(i-2) + 35*a(i-3)
                    - 35*a(i-4) + 21*a(i-5) - 7*a(i-6) + a(i-7)   */
                seq_t t1 = (seq_t)7  * cache[i-2];
                seq_t t2 = (seq_t)21 * cache[i-3];
                seq_t t3 = (seq_t)35 * cache[i-4];
                seq_t t4 = (seq_t)35 * cache[i-5];
                seq_t t5 = (seq_t)21 * cache[i-6];
                seq_t t6 = (seq_t)7  * cache[i-7];
                seq_t t7 =          cache[i-8];

                seq_t val = t1;
                val -= t2;
                val += t3;
                val -= t4;
                val += t5;
                val -= t6;
                val += t7;
                cache[i-1] = val;
            }
        }
    }
    cache_len = n;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;            /* undefined for n<1, return 0 */
    ensure_cache(n);
    return cache[n-1];
}
