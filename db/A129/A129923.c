/*
 * A129923  a(n) = (n+5)! / 5.
 * https://oeis.org/A129923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+5)!/5 Recurrence: a(0) = 24; a(n) = (n+5) * a(n-1) for
 *            n ≥ 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache   = NULL;   /* array of computed a(n) values      */
static size_t  a_cap    = 0;       /* allocated capacity of the array    */
static size_t  a_len    = 0;       /* number of values stored (0 .. a_len-1) */

/* Ensure the cache array exists and contains a(0) */
static void init_cache(void)
{
    if (a_cache != NULL) return;
    a_cap = 16;
    a_cache = (seq_t *)malloc(a_cap * sizeof(seq_t));
    if (!a_cache) exit(1);               /* allocation failure aborts */
    a_cache[0] = (seq_t)24;              /* a(0) = 24   */
    a_len   = 1;
}

/* Grow the cache array until it can hold 'needed' entries */
static void ensure_can_hold(size_t needed)
{
    if (needed <= a_cap) return;
    size_t new_cap = a_cap ? a_cap : 1;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);                   /* allocation failure aborts */
    a_cache = tmp;
    a_cap   = new_cap;
}

/* Compute a(n) */
seq_t a(idx_t n)
{
    init_cache();                       /* make sure cache is initialized */

    /* Extend the cache up to index n if necessary */
    if ((size_t)n >= a_len) {
        size_t needed = (size_t)n + 1;
        ensure_can_hold(needed);
        for (idx_t k = (idx_t)a_len; k <= n; ++k) {
            seq_t factor = (seq_t)(k + 5);             /* recurrence factor */
            a_cache[(size_t)k] = a_cache[(size_t)k - 1] * factor;
        }
        a_len = needed;
    }

    return a_cache[(size_t)n];
}
