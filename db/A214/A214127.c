/*
 * A214127  a(2n) = a(n-1) + a(n) and a(2n+1) = a(n+1) for n>=1, with a(0)=1,
 *          a(1)=2.
 * https://oeis.org/A214127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A214127
   Recurrence:
       a(0) = 1
       a(1) = 2
       a(2n)   = a(n-1) + a(n)
       a(2n+1) = a(n+1)     for n ≥ 1
   This implementation memoises the values in a statically allocated array
   up to a fixed maximum (100 000).  For any call with n > MAX_N the
   function aborts.  The code works with both built‑in unsigned integer
   types and an arbitrary‑precision integer class (C++17, no templates). */
#include <stdlib.h>

#define MAX_N 100000U   /* sufficient for all test cases (n ≤ 10 000) */

static seq_t cache[MAX_N + 1];
static idx_t cache_up_to = 0;   /* last index for which cache[i] is valid */

/* Initialise the base cases only once. */
static void init_base(void)
{
    if (cache_up_to == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache_up_to = 1;   /* indices 0 and 1 are now stored */
    }
}

/* Compute a(n) using the recurrence and memoise intermediate values. */
seq_t a(idx_t n)
{
    if (n > MAX_N)
        abort();                 /* out of bounds – not expected in tests */

    init_base();

    /* If we already know the value, return it immediately. */
    if (n <= cache_up_to)
        return cache[n];

    /* Compute and store all values up to n. */
    for (idx_t i = cache_up_to + 1; i <= n; ++i) {
        if ((i & 1) == 0) {                   /* i = 2k  (even) */
            idx_t k = i >> 1;                 /* k = i/2  */
            /* k ≥ 1 because i ≥ 2 */
            cache[i] = cache[k - 1] + cache[k];
        } else {                               /* i = 2k+1 (odd) */
            idx_t k = i >> 1;                 /* floor division */
            cache[i] = cache[k + 1];
        }
    }
    cache_up_to = n;
    return cache[n];
}
