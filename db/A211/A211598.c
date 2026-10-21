/*
 * A211598  Number of -2..2 arrays x(i) of n+1 elements i=1..n+1 with
 *          set{t,u,v in 0,1}((x[i+t]+x[j+u]+x[k+v])*(-1)^(t+u+v)) having
 *          four, five, six, seven or eight distinct values for every
 *          i,j,k<=n.
 * https://oeis.org/A211598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 22, u64 n <= 47, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A211598
   Empirical linear recurrence (valid for n ≥ 14)
   a(n) = 2*a(n-1) +16*a(n-2) -31*a(n-3) -97*a(n-4)
          +178*a(n-5) +284*a(n-6) -473*a(n-7) -418*a(n-8)
          +578*a(n-9) +288*a(n-10) -270*a(n-11) -72*a(n-12)
          +36*a(n-13)
   Initial values for n = 1 … 13:
   16, 48, 128, 322, 780, 1874, 4400, 10458,
   24332, 57922, 134744, 322650, 753100
*/

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* seq_t and idx_t are supplied by the build system; no typedefs here. */

/* Table of initial values (index 0 unused). */
static const seq_t init_values[14] = {
    0,          /* unused */
    16, 48, 128, 322, 780, 1874, 4400,
    10458, 24332, 57922, 134744, 322650, 753100
};

/* Static cache of computed values.  We grow it lazily. */
static seq_t *cache          = NULL;   /* array: cache[0] unused */
static size_t cache_len      = 0;     /* allocated length (>=n+1) */
static idx_t  cache_last     = 0;     /* largest n for which cache[n] is valid */

/* Ensure that cache[0..n] exists and is filled up to n. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_last)
        return;

    /* Expand allocation if necessary */
    if ((size_t)n + 1 > cache_len) {
        size_t new_len = cache_len ? cache_len * 2 : 64;
        if (new_len < (size_t)n + 1)
            new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        cache = tmp;
        cache_len = new_len;
        /* Initialise new slots to zero (cumulative sum may be large). */
        for (size_t i = cache_last + 1; i < new_len; ++i)
            cache[i] = 0;
    }

    /* Fill initial values for 1 … 13 if requested */
    if (cache_last < 13) {
        idx_t upto = n < 13 ? n : 13;
        for (idx_t i = cache_last + 1; i <= upto; ++i)
            cache[i] = init_values[i];
        if (upto > cache_last)
            cache_last = upto;
    }

    /* Compute further terms using the recurrence */
    idx_t start = cache_last + 1;
    if (start < 14)
        start = 14;
    for (idx_t i = start; i <= n; ++i) {
        seq_t val = 0;
        val += 2   * cache[i - 1];
        val += 16  * cache[i - 2];
        val -= 31  * cache[i - 3];
        val -= 97  * cache[i - 4];
        val += 178 * cache[i - 5];
        val += 284 * cache[i - 6];
        val -= 473 * cache[i - 7];
        val -= 418 * cache[i - 8];
        val += 578 * cache[i - 9];
        val += 288 * cache[i -10];
        val -= 270 * cache[i -11];
        val -= 72  * cache[i -12];
        val += 36  * cache[i -13];
        cache[i] = val;
    }
    cache_last = n;
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* sequence is defined for n ≥ 1 */
    ensure_cache(n);
    return cache[n];
}
