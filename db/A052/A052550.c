/*
 * A052550  Expansion of (1-2*x)/(1 - 3*x - x^2 + 2*x^3).
 * https://oeis.org/A052550
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) + a(n-2) - 2*a(n-3) with a(0)=1, a(1)=1, a(2)=4
 *            for n>2
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 39, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;   /* array storing computed terms   */
static idx_t a_cache_len = 0;   /* number of elements currently in the array  */

/* Ensure that a_cache contains terms up to index n (inclusive). */
static void
ensure_cache(idx_t n)
{
    if (a_cache_len > n)                     /* already have it   */
        return;

    idx_t new_len = n + 1;                    /* at least n+1      */
    /* allocate memory for the new size */
    a_cache = (seq_t *)realloc(a_cache, new_len * sizeof(seq_t));

    /* initialise new entries */
    for (idx_t i = a_cache_len; i < new_len; ++i) {
        if (i == 0)          a_cache[i] = (seq_t)1;
        else if (i == 1)      a_cache[i] = (seq_t)1;
        else if (i == 2)      a_cache[i] = (seq_t)4;
        else                  a_cache[i] = 3*a_cache[i-1] + a_cache[i-2] - 2*a_cache[i-3];
    }
    a_cache_len = new_len;
}

/* ------------------------------------------------------------ */
seq_t
a(idx_t n)
{
    ensure_cache(n);
    return a_cache[n];
}
