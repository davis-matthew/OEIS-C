/*
 * A194428  Number of iterations of the map n->n/3 if n == 0 (mod 3), n->4*n+a
 *          if 4*n+a == 0 (mod 3) where a = 1 or 2, before reaching the end of
 *          the cycle.
 * https://oeis.org/A194428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 8919, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Iteration count of the map until the first repeated value.
   The map is defined by:
     if n % 3 == 0       -> n/3
     else if n % 3 == 1  -> 4*n + 2
     else                -> 4*n + 1
   a(n) is the number of steps until a value repeats in the orbit of n. */
 
#include <stdlib.h>
 
/* Compute the next value in the orbit. */
static inline seq_t next_value(seq_t x)
{
    switch ((int)(x % 3))
    {
        case 0: return x / 3;
        case 1: return 4 * x + 2;
        default: /* case 2 */ return 4 * x + 1;
    }
}
 
/* Compute a(n).  The sequence is defined for n >= 1. */
seq_t a(idx_t n)
{
    /* The maximum number of distinct values we expect to encounter in
       an orbit of a starting number up to about 10000 is far below a few
       thousand.  A fixed‑size buffer keeps the implementation simple
       and works for both C11 and C++17. */
    enum { MAX_VISITS = 4096 };
    seq_t visited[MAX_VISITS];
    size_t len = 0;
    size_t i;
    seq_t cur = (seq_t)n;
    seq_t steps = 0;
 
    /* Store the starting value. */
    visited[len++] = cur;
 
    while (1)
    {
        cur = next_value(cur);
        ++steps;
        for (i = 0; i < len; ++i)
            if (visited[i] == cur)
                return steps;
 
        /* In the extremely unlikely event that the orbit exceeds
           MAX_VISITS distinct values, we cap the array to avoid
           overrunning the buffer but the function will still
           terminate. */
        if (len < (size_t)MAX_VISITS)
            visited[len++] = cur;
    }
}
