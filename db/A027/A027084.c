/*
 * A027084  G.f.: x^2*(x^2 + x + 1)/(x^4 - 2*x + 1).
 * https://oeis.org/A027084
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (t(n) + t(n+2) - 3)/2, where t(i) are Tribonacci numbers
 *            with t(1)=1, t(2)=1, t(3)=2
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Trivial cache of Tribonacci values.  t[1]..t[trib_filled] are valid. */
static seq_t *t_cache = NULL;   /* dynamic array of tribonacci numbers */
static idx_t trib_filled = 0;   /* highest index already computed */
static size_t t_cap = 0;        /* allocated capacity (indices 0..t_cap-1) */

/* Ensure that t_cache contains values up to index `need`. */
static void ensure_trib(idx_t need)
{
    if (need <= trib_filled)
        return;

    /* Grow the array if necessary. */
    size_t new_cap = t_cap ? t_cap : 16;
    while (new_cap <= need)
        new_cap <<= 1;

    seq_t *new_arr = (seq_t *)realloc(t_cache, new_cap * sizeof(seq_t));
    if (!new_arr)
        abort();                 /* allocation failed */

    t_cache = new_arr;
    t_cap   = new_cap;

    /* Initialise the first tribonacci values if this is the first run. */
    if (trib_filled == 0) {
        t_cache[1] = (seq_t)1;
        t_cache[2] = (seq_t)1;
        t_cache[3] = (seq_t)2;
        trib_filled = 3;
    }

    /* Compute missing Tribonacci numbers. */
    for (idx_t i = trib_filled + 1; i <= need; ++i) {
        t_cache[i] = t_cache[i - 1] + t_cache[i - 2] + t_cache[i - 3];
    }
    trib_filled = need;
}

/* External function required by the framework. */
seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;          /* sequence is undefined for n < 2 */

    idx_t need = n + 2;           /* We need t(n) and t(n+2) */
    ensure_trib(need);

    seq_t result = (t_cache[n] + t_cache[n + 2] - (seq_t)3) / (seq_t)2;
    return result;
}
