/*
 * A208949  Number of 7-bead necklaces labeled with numbers -n..n not allowing
 *          reversal, with sum zero with no three beads in a row equal.
 * https://oeis.org/A208949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 10*a(n-2) + 11*a(n-3) - 10*a(n-4) + 11*a(n-5)
 *            * - 9*a(n-6) + 9*a(n-8) - 11*a(n-9) + 10*a(n-10) * - 11*a(n-11)
 *            + 10*a(n-12) - 5*a(n-13) + a(n-14) * (Empirical recurrence;
 *            Initial terms: a(1)=38, a(2)=988, … a(14)=43219580)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 30, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* The following types are provided by the build system:
   typedef <int_type> seq_t;
   typedef <unsigned> idx_t;
*/

static seq_t *cache = NULL;      /* cache[i] holds a(i+1) */
static size_t cache_len = 0;     /* number of terms currently cached */
static size_t cache_cap = 0;     /* allocated capacity of cache */

static const seq_t init_vals[14] = {
    (seq_t)38,        (seq_t)988,       (seq_t)7974,
    (seq_t)37044,     (seq_t)125322,    (seq_t)344456,
    (seq_t)817362,    (seq_t)1738516,   (seq_t)3397474,
    (seq_t)6205668,   (seq_t)10726862,  (seq_t)17710724,
    (seq_t)28129694,  (seq_t)43219580
};

/* Ensure cache contains terms up to n (1‑based). */
static void ensure_cache(idx_t n)
{
    if (n <= cache_len)
        return;

    /* Determine required capacity */
    size_t need = (size_t)n;
    if (cache_len == 0 && need < 14)
        need = 14;   /* we need at least 14 slots for the initial values */

    if (need > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap < need)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();       /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise first 14 terms if the cache is empty */
    if (cache_len == 0) {
        for (int i = 0; i < 14; ++i)
            cache[i] = init_vals[i];
        cache_len = 14;
    }

    /* Compute remaining terms using the recurrence */
    for (size_t i = cache_len; i < (size_t)n; ++i) {
        seq_t pos = 0;
        seq_t neg = 0;

        /* Positive coefficients */
        pos += 5  * cache[i - 1];
        pos += 11 * cache[i - 3];
        pos += 11 * cache[i - 5];
        pos += 9  * cache[i - 8];
        pos += 10 * cache[i -10];
        pos += 10 * cache[i -12];   /* coefficient for a(n-12) is +10 */
        pos += 1  * cache[i -14];

        /* Negative coefficients */
        neg += 10 * cache[i - 2];
        neg += 10 * cache[i - 4];
        neg += 9  * cache[i - 6];
        neg += 11 * cache[i - 9];
        neg += 11 * cache[i -11];
        neg += 5  * cache[i -13];   /* coefficient for a(n-13) is -5 */

        cache[i] = pos - neg;
    }

    cache_len = (size_t)n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* undefined for n=0, but return 0 for safety */

    if (n <= 14)
        return init_vals[n - 1];

    ensure_cache(n);
    return cache[n - 1];
}
