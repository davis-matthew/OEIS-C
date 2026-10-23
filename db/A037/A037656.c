/*
 * A037656  Base 7 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,0,2.
 * https://oeis.org/A037656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(n)=a(n-1)*7+d_n for n>=1,
   where d_n = 3,0,2,3,0,2,… (period 3) */
#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cached = 0;

/* Ensure cache[0..n] is computed */
static void ensure_cache(idx_t n)
{
    if (n <= cached)
        return;
    if (cache == NULL)
        cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    else
        cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));

    if (cached == 0)
        cache[0] = 0; /* base case mapping 0*7+0=0, not used for a(1) */

    for (idx_t i = cached + 1; i <= n; ++i) {
        idx_t rem = i % 3;
        seq_t digit;
        if (rem == 1)
            digit = 3;   /* first term of period */
        else if (rem == 2)
            digit = 0;   /* second term of period */
        else           /* rem == 0 */
            digit = 2;   /* third term of period */
        cache[i] = cache[i - 1] * 7 + digit;
    }
    cached = n;
}

seq_t a(idx_t n)
{
    /* Sequence is defined for n >= 1; behaviour for n < 1 is irrelevant. */
    ensure_cache(n);
    return cache[n];
}
