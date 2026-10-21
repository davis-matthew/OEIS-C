/*
 * A227104  a(0)=-1, a(1)=3; a(n+2) = a(n+1) + a(n) + 2*A057078(n+1).
 * https://oeis.org/A227104
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 10, i16 n <= 21, i32 n <= 39, i64 n <= 39, i128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Formula used: a(0) = -1, a(1) = 3, a(2) = 2,
   a(n) = a(n-3) + 4*F(n-2)  for n ≥ 3,
   where F(k) is the Fibonacci number with F(0)=0, F(1)=1.
   This is equivalent to the OEIS recurrence a(n+3) = a(n)+4*F(n+1).
*/

#include <stdlib.h>

static seq_t *a_cache   = NULL;   /* cache of a-values      */
static seq_t *fib_cache = NULL;   /* cache of Fibonacci    */
static idx_t  a_cap     = 0;       /* current capacity of a */
static idx_t  fib_cap   = 0;       /* current capacity of fib */

/* Ensure fib_cache has at least “new_cap” elements, indexed 0..new_cap-1.
   F[0] = 0, F[1] = 1. */
static void grow_fib(idx_t new_cap)
{
    if (new_cap <= fib_cap) return;
    seq_t *tmp = (seq_t *)realloc(fib_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                             /* allocation failure */
    fib_cache = tmp;

    /* Initialise the roots of the Fibonacci sequence if needed. */
    if (fib_cap == 0 && new_cap > 0) fib_cache[0] = (seq_t)0;
    if (fib_cap <= 1 && new_cap > 1) fib_cache[1] = (seq_t)1;

    /* Compute missing Fibonacci values.  Start at the largest index we
       already have, but at least 2 to avoid accessing negative indices. */
    idx_t i = fib_cap;
    if (i < 2) i = 2;
    for (; i < new_cap; ++i)
        fib_cache[i] = fib_cache[i-1] + fib_cache[i-2];

    fib_cap = new_cap;
}

/* Ensure a_cache has at least “new_cap” elements, indexed 0..new_cap-1. */
static void grow_a(idx_t new_cap)
{
    if (new_cap <= a_cap) return;
    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    a_cache = tmp;

    /* Base cases. */
    if (new_cap > 0) a_cache[0] = (seq_t)-1;
    if (new_cap > 1) a_cache[1] = (seq_t)3;
    if (new_cap > 2) a_cache[2] = (seq_t)2;

    /* Compute new terms using the recurrence a(n) = a(n-3)+4*F(n-2). */
    idx_t start = a_cap > 3 ? a_cap : 3;
    for (idx_t i = start; i < new_cap; ++i)
        a_cache[i] = a_cache[i-3] + (seq_t)4 * fib_cache[i-2];

    a_cap = new_cap;
}

/* Return the n‑th term of A227104. */
seq_t a(idx_t n)
{
    /* Fibonacci values up to F(n-1) are needed; grow by a couple for safety. */
    grow_fib(n + 2);
    grow_a(n + 1);
    return a_cache[n];
}
