/*
 * A065113  Sum of the squares of the a(n)-th and the (a(n)+1)st triangular
 *          numbers (A000217) is a perfect square.
 * https://oeis.org/A065113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - a(n-2) + 4, a(1)=6, a(2)=40
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* memoized recurrence for A065113 */
static seq_t *a_cache = NULL;        /* storage for computed terms   */
static idx_t a_cache_cap = 0;        /* allocated capacity (elements)*/
static idx_t a_cache_len = 0;        /* number of terms stored       */

/* grow the cache to hold at least `min_cap` entries */
static void ensure_cache(idx_t min_cap)
{
    if (a_cache_cap >= min_cap) return;
    idx_t new_cap = a_cache_cap ? a_cache_cap << 1 : 4;
    while (new_cap < min_cap) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();               /* allocation failure is fatal   */
    a_cache = tmp;
    a_cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* undefined for n<1, safe fallback */

    /* initialise first two terms on first use */
    if (a_cache_len == 0) {
        ensure_cache(2);
        a_cache[0] = (seq_t)6;
        a_cache[1] = (seq_t)40;
        a_cache_len = 2;
    }

    if (n <= a_cache_len) return a_cache[n-1];

    /* make sure we have enough space */
    ensure_cache(n);

    /* iterative construction from cached end */
    seq_t a_nm2 = a_cache[a_cache_len-2]; /* a(n-2) */
    seq_t a_nm1 = a_cache[a_cache_len-1]; /* a(n-1) */

    for (idx_t i = a_cache_len + 1; i <= n; ++i) {
        seq_t a_n = (seq_t)6 * a_nm1 - a_nm2 + (seq_t)4;
        a_cache[a_cache_len] = a_n;
        ++a_cache_len;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_cache[n-1];
}
