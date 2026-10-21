/*
 * A082570  a(1)=1, a(n)=ceiling(n*(a(n-1)+1/a(n-1))).
 * https://oeis.org/A082570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(n*(a(n-1)+1/a(n-1)))
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for built‑in integer types if needed */

/* Forward declaration of seq_t and idx_t; these are supplied by the build. */

static seq_t a_cache[10001];          /* a(1)...a(10000) */
static idx_t a_cached = 0;            /* number of terms currently cached */

/* Compute the nth term of OEIS A082570 (1‑based index).  Works for any
   seq_t that supports + - * / with truncating division. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* the offset is 1, n must be >=1 */

    /* initialise cache with a(1) */
    if (a_cached == 0) {
        a_cache[0] = (seq_t)1;       /* a(1) */
        a_cached  = 1;
    }

    /* extend cache up to the requested index, if needed */
    while (a_cached < n) {
        /* previous term a(k) where k = a_cached */
        seq_t prev = a_cache[a_cached - 1];
        idx_t  k   = a_cached + 1;   /* compute a(k) */

        /* compute term1 = k * prev   (integer multiplication) */
        seq_t term1 = prev * (seq_t)k;

        /* compute term2 = ceil(k / prev) = (k + prev - 1) / prev */
        seq_t term2 = ((seq_t)k + prev - 1) / prev;

        seq_t val = term1 + term2;   /* a(k) */
        a_cache[a_cached] = val;

        a_cached = k;
    }

    return a_cache[n - 1];
}
