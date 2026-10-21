/*
 * A096978  Sum of the areas of the first n Jacobsthal rectangles.
 * https://oeis.org/A096978
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A096978: Sum of the areas of the first n Jacobsthal rectangles
 * Recurrence: a(0)=0, a(1)=1, a(2)=4, a(3)=19
 *              a(n)=4*a(n-1)+3*a(n-2)-14*a(n-3)+8*a(n-4)   for n>=4
 * This implementation memoises computed terms so any order of calls works
 * and each call is O(1) after the first expansion beyond the requested n. */

#include <stdlib.h>   /* for realloc, NULL */

static seq_t *a_cache = NULL;      /* dynamic cache of computed terms          */
static size_t a_cached = 0;        /* number of terms currently stored (size)  */

/* Ensure that a_cache contains terms up to at least n (inclusive). */
static void ensure_a(size_t n)
{
    if (n < a_cached)
        return;   /* already computed */

    size_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
    if (!tmp)
        return;   /* allocation failed; caller will crash silently as per build policy */

    a_cache = tmp;

    /* Initialise new terms */
    for (size_t i = a_cached; i <= n; ++i) {
        if (i == 0)            a_cache[i] = 0;
        else if (i == 1)       a_cache[i] = 1;
        else if (i == 2)       a_cache[i] = 4;
        else if (i == 3)       a_cache[i] = 19;
        else {
            /* Use unsigned arithmetic; intermediate sum >= subtraction value
             * due to growth of the sequence, so no underflow occurs. */
            seq_t term = 4 * a_cache[i-1];
            term += 3 * a_cache[i-2];
            term -= 14 * a_cache[i-3];
            term += 8 * a_cache[i-4];
            a_cache[i] = term;
        }
    }

    a_cached = new_size;
}

seq_t a(idx_t n)
{
    /* Convert idx_t (uint64_t) to size_t; safe for n <= size_t max. */
    size_t idx = (size_t)n;
    ensure_a(idx);
    return a_cache[idx];
}
