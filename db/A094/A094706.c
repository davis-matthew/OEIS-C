/*
 * A094706  Convolution of Pell(n) and 2^n.
 * https://oeis.org/A094706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 24, u64 n <= 49, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A094706: Convolution of Pell(n) and 2^n.
   Recurrence: a(0)=0, a(1)=1, a(2)=4,
                a(n)=4*a(n-1) - 3*a(n-2) - 2*a(n-3)   for n≥3
*/

#include <stdlib.h>

static seq_t *a_cache = NULL;     /* cache of computed values */
static idx_t   a_cache_cap = 0;   /* capacity of the cache array   */
static idx_t   a_cache_size = 0;  /* number of valid entries in cache */

/* Make sure that the cache contains values up to index n, and that
   a_cache[i] is defined for all i <= n.   */
static void ensure_a_upto(idx_t n)
{
    /* Grow the array if needed.  The cast is required to compile
       under C++ when seq_t is an arbitrary‑precision class.   */
    if (a_cache_cap <= n) {
        idx_t new_cap = (a_cache_cap == 0) ? 8 : (a_cache_cap << 1);
        while (new_cap <= n) new_cap <<= 1;
        a_cache = (seq_t*)realloc((void*)a_cache, new_cap * sizeof(seq_t));
        a_cache_cap = new_cap;
    }

    /* If the cache is already large enough, nothing more to do. */
    if (a_cache_size > n) return;

    /* Initialise the first three terms if they are not in the cache. */
    if (a_cache_size == 0) {
        a_cache[0] = 0;
        a_cache[1] = 1;
        a_cache[2] = 4;
        a_cache_size = 3;
    }

    /* Compute any missing terms.  The recurrence uses the
       already computed values a_cache[i-1], a_cache[i-2],
       and a_cache[i-3].                                         */
    for (idx_t i = a_cache_size; i <= n; ++i) {
        seq_t value;
        if (i == 0)        value = 0;
        else if (i == 1)   value = 1;
        else if (i == 2)   value = 4;
        else {
            value = 4 * a_cache[i-1];
            value -= 3 * a_cache[i-2];
            value -= 2 * a_cache[i-3];
        }
        a_cache[i] = value;
    }
    a_cache_size = n + 1;
}

seq_t a(idx_t n)
{
    ensure_a_upto(n);
    return a_cache[n];
}
