/*
 * A042028  Numerators of continued fraction convergents to sqrt(538).
 * https://oeis.org/A042028
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Numerators of convergents to sqrt(538).
   Recurrence:
       p[-2] = 0, p[-1] = 1
       p[0]  = 23
       p[1]  = 5*p[0] + 1
       for n>=2: p[n] = period[(n-1)%7]*p[n-1] + p[n-2]
   where period = {5,7,1,1,7,5,46} (periodic coefficients after the
   initial term 23). */

static seq_t *cf_cache = NULL;           /* cache of numerators p[n] */
static size_t cf_cached = 0;             /* number of entries cached (cf_cached = last_index + 1) */

static const int period[7] = {5, 7, 1, 1, 7, 5, 46};

static void cf_ensure(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (cf_cached > n) return;          /* already available */

    /* enlarge the cache */
    seq_t *new_cache = (seq_t *)realloc(cf_cache, need * sizeof(seq_t));
    if (!new_cache) abort();
    cf_cache = new_cache;

    /* initialise the first term if first call */
    if (cf_cached == 0) {
        cf_cache[0] = (seq_t)23;
        cf_cached = 1;
    }

    /* initialise p[1] separately because it needs p[-1] = 1 */
    if (cf_cached <= 1 && (size_t)n >= 1) {
        cf_cache[1] = (seq_t)period[0] * cf_cache[0] + (seq_t)1;
        cf_cached = 2;
    }

    /* compute remaining entries */
    for (size_t i = cf_cached; i <= (size_t)n; ++i) {
        int coeff = period[(i - 1) % 7];
        cf_cache[i] = (seq_t)coeff * cf_cache[i - 1] + cf_cache[i - 2];
    }

    cf_cached = need;
}

seq_t a(idx_t n)
{
    cf_ensure(n);
    return cf_cache[(size_t)n];
}
