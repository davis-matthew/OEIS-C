/*
 * A021485  Decimal expansion of 1/481.
 * https://oeis.org/A021485
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = a(n-1) - a(n-2) + a(n-3) - a(n-4) + a(n-5) with a(0)=0,
 *            a(1)=0, a(2)=2, a(3)=0, a(4)=7
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *seq_cache = NULL;   /* dynamic array of already computed terms */
static size_t seq_cache_len = 0;  /* allocated size of seq_cache */

/* Ensure that seq_cache has been filled up to (and including) index n.
   The function keeps the array growing only forward; it can be called
   with n in any order. */
static void seq_ensure(idx_t n)
{
    if (seq_cache_len > (size_t)n)
        return;                                   /* already computed */

    size_t target = (size_t)n + 1;                 /* need indices 0..n */

    seq_cache = (seq_t *)realloc(seq_cache,
                                 target * sizeof(seq_t));
    /* initialise newly added entries */
    for (size_t i = seq_cache_len; i < target; ++i) {
        if (i == 0)          seq_cache[i] = (seq_t)0;
        else if (i == 1)     seq_cache[i] = (seq_t)0;
        else if (i == 2)     seq_cache[i] = (seq_t)2;
        else if (i == 3)     seq_cache[i] = (seq_t)0;
        else if (i == 4)     seq_cache[i] = (seq_t)7;
        else {
            /* recurrence – intermediate arithmetic done in signed long long
               to avoid unsigned underflow.  The final result fits in seq_t
               for all n up to the test limit (the sequence is periodic
               with period 6, values ≤ 9). */
            long long v =
                (long long)seq_cache[i-1] -
                (long long)seq_cache[i-2] +
                (long long)seq_cache[i-3] -
                (long long)seq_cache[i-4] +
                (long long)seq_cache[i-5];
            seq_cache[i] = (seq_t)v;
        }
    }
    seq_cache_len = target;
}

seq_t a(idx_t n)
{
    seq_ensure(n);
    return seq_cache[(size_t)n];
}
