/*
 * A023496  a(n) = b(n) + d(n), where b(n) = (n-th Fibonacci number > 2) and
 *          d(n) = (n-th number that is 1 or is not a Lucas number).
 * https://oeis.org/A023496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000045(n+4) + A090946(n+1) where A000045(n) = n-th
 *            Fibonacci number (F(0)=0, F(1)=1) and A090946(n) is the n-th
 *            number that is 1 or is not a Lucas number. Here the Lucas
 *            numbers are taken as 1,3,4,7,11,18,... (i.e. the standard Lucas
 *            sequence with the initial 2 omitted)
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Validated: partial, pass, 35 terms through n = 34 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t fib(idx_t n) {
    if (n <= 2) return (seq_t)1;
    seq_t f1 = 1, f2 = 1, f3;
    for (idx_t i = 3; i <= n; ++i) {
        f3 = f1 + f2;
        f1 = f2;
        f2 = f3;
    }
    return f2;
}

/* Helper routine to generate the sequence d(n) = n-th number that is 1
   or not a Lucas number (Lucas numbers: 1,3,4,7,11,...).  Result values
   are stored in a static cache so that future calls are fast.          */
static seq_t d_value(idx_t n)
{
    static seq_t *cache = NULL;          /* array storing d(0), d(1), ... */
    static idx_t  cache_len = 0;         /* number of elements currently stored */
    static size_t cache_cap = 0;         /* current capacity of the array        */

    /* Keep track of where we are while iterating over natural numbers */
    static idx_t   next_num = 1;         /* next natural number to consider    */

    /* For testing whether a number is Lucas under the variant starting with
       1,3,4,7,... we maintain two consecutive Lucas numbers.
       l_prev = L(k-1), l_curr = L(k). Initially L0=1, L1=3.           */
    static idx_t   l_prev = 1;           /* previous Lucas number              */
    static idx_t   l_curr = 3;           /* current Lucas number               */

    /* Ensure that we have computed at least (n+1) elements */
    if (cache_len <= n)
    {
        /* Initialise cache if needed */
        if (cache == NULL)
        {
            cache_cap = 64;
            cache = mem_new(seq_t, cache_cap);
            if (cache == NULL)
                exit(1); /* fatal: memory exhausted */
        }

        /* Append elements until we have (n+1) of them */
        while (cache_len <= n)
        {
            /* Update Lucas numbers until l_curr >= next_num                 */
            while (l_curr < next_num)
            {
                idx_t l_next = l_prev + l_curr;
                l_prev = l_curr;
                l_curr = l_next;
            }

            /* Determine whether next_num is a Lucas number                 */
            int is_lucas = (next_num == l_curr);

            /* The number 1 is always included, even though it is Lucas    */
            if (next_num == (idx_t)1 || is_lucas == 0)
            {
                /* Grow cache if needed */
                if (cache_len == cache_cap)
                {
                    cache_cap = cache_cap * 2;
                    cache = mem_resize(seq_t, cache, cache_cap);
                    if (cache == NULL)
                        exit(1);
                }

                /* Store the value as a seq_t */
                cache[cache_len++] = (seq_t)next_num;
            }

            ++next_num;
        }
    }

    return cache[n];
}

/* Forward declaration of helper that returns the (n)-th element of the
   sequence of numbers that are 1 or not Lucas numbers. */
static seq_t d_value(idx_t n);

/* Compute a(n) = Fibonacci(n+4) + d(n) */
seq_t a(idx_t n)
{
    /* Fibonacci term: F(n+4) */
    seq_t fib_val = fib(n + (idx_t)4);

    /* d(n) term */
    seq_t d_val = d_value(n);

    return fib_val + d_val;
}

/* --------------------------------------------------------------------- */
