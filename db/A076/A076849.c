/*
 * A076849  Let u(1)=1, u(n) = n + abs(u(ceiling(n/2)) - u(floor(n/2))); then
 *          a(n) = u(n) - n.
 * https://oeis.org/A076849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = u(n) - n where u(1) = 1 and for n ≥ 2: u(n) = n + abs(
 *            u(⌈n/2⌉) - u(⌊n/2⌋) ) The function below computes this
 *            recurrence with memoisation to handle arbitrary call order
 *            efficiently
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *u_cache = NULL;     /* memoisation table for u(n) */
static idx_t cache_size = 0;     /* current allocated size of u_cache */

/* Recursive helper: compute u(n) and store it in u_cache if not already done */
static seq_t u_recursive(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* Return cached value if available */
    if (n < cache_size && u_cache[n] != 0)
        return u_cache[n];

    /* Compute u(ceil(n/2)) and u(floor(n/2)) */
    seq_t u_left  = u_recursive((n + 1) / 2);
    seq_t u_right = u_recursive(n / 2);

    /* absolute difference */
    seq_t diff = (u_left >= u_right) ? (u_left - u_right) : (u_right - u_left);

    /* u(n) = n + |u_left - u_right| */
    seq_t u_val = (seq_t)n + diff;

    /* Resize cache if necessary */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(u_cache, new_size * sizeof(seq_t));
        if (!new_cache)   /* abort on allocation failure */
            abort();
        /* initialise new entries to 0 */
        for (idx_t i = cache_size; i < new_size; ++i)
            new_cache[i] = 0;
        u_cache = new_cache;
        cache_size = new_size;
    }

    /* Store computed value */
    u_cache[n] = u_val;
    return u_val;
}

/* Public API required by the build */
seq_t a(idx_t n)
{
    seq_t u_val = u_recursive(n);
    return u_val - (seq_t)n;
}
