/*
 * A165177  Number of reduced words of length n in Coxeter group on 45
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^8 = I.
 * https://oeis.org/A165177
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A165177
   Recurrence:  a(0)=1, a(1)=45, a(2)=1980, a(3)=87120, a(4)=3833280,
   a(5)=168664320, a(6)=7421230080, a(7)=326534123520, a(8)=14367501433890
   For n≥9, a(n) = 43*(a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5)+a(n-6)+a(n-7))
               - 946*a(n-8) + p(n)
   where the stretch‑term p(n) equals 1 for n=0 and n=8,
   equals 2 for 1≤n≤7, and equals 0 for n>8.  */

#include <stdlib.h>

static seq_t *cache = NULL;        /* memoisation array */
static idx_t cache_cap = 0;        /* current allocated size   */
static idx_t next_compute = 0;     /* index of first memoised value not yet computed */

/* predefined stretch‑term p(n) for 0≤n≤8 */
static const seq_t p_term[9] = {
    (seq_t)1, /* n = 0 */
    (seq_t)2, /* n = 1 */
    (seq_t)2, /* n = 2 */
    (seq_t)2, /* n = 3 */
    (seq_t)2, /* n = 4 */
    (seq_t)2, /* n = 5 */
    (seq_t)2, /* n = 6 */
    (seq_t)2, /* n = 7 */
    (seq_t)1  /* n = 8 */
};

seq_t a(idx_t n)
{
    /* Grow the cache if necessary */
    if (cache_cap <= n) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* out of memory; return 0 */
        /* initialise newly allocated slots to zero (optional) */
        for (idx_t i = cache_cap; i < new_cap; ++i) tmp[i] = (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute all values up to n that are not yet cached */
    for (idx_t i = next_compute; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        idx_t max_k = (i < 7) ? i : 7;     /* number of previous terms to include */

        for (idx_t k = 1; k <= max_k; ++k) sum += cache[i - k];

        seq_t val = sum * (seq_t)43;
        if (i >= 8) val -= (seq_t)946 * cache[i - 8];

        if (i <= 8) val += p_term[i];

        cache[i] = val;
    }

    next_compute = n + 1;                 /* all values up to n are now memoised */
    return cache[n];
}
