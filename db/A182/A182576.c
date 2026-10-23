/*
 * A182576  Number of 1's in the Zeckendorf representation of n^2.
 * https://oeis.org/A182576
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   number of 1's in the Zeckendorf representation of n^2
 * Range:     u8 n <= 15, u16 n <= 380, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Lazy Fibonacci cache: F(2)=1, F(3)=2, F(4)=3, ... */
static seq_t *fibs   = NULL;   /* array of Fibonacci numbers */
static size_t fib_cnt = 0;     /* number of Fibonacci numbers stored */

/* Grow fibs until fibs[fib_cnt-1] > limit.  Uses malloc/realloc with
   explicit casts so the code compiles both in C and C++. */
static void ensure_fib_upto(seq_t limit)
{
    /* Initialise first time. */
    if (fibs == NULL)
    {
        size_t init_cap = 8;
        fibs = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (fibs == NULL) return;          /* unlikely, but safe */
        fibs[0] = (seq_t)1;
        fibs[1] = (seq_t)2;
        fib_cnt = 2;
    }

    /* Grow until the last stored Fibonacci number exceeds 'limit'. */
    while (fib_cnt >= 2 && fibs[fib_cnt - 1] <= limit)
    {
        seq_t next = fibs[fib_cnt - 1] + fibs[fib_cnt - 2];
        /* Stop if addition overflowed and 'next' is smaller. */
        if (next < fibs[fib_cnt - 1]) break;

        size_t new_cap = fib_cnt * 2;
        seq_t *tmp = (seq_t *)realloc(fibs, new_cap * sizeof(seq_t));
        if (tmp == NULL) break;           /* allocation failure – stay short */
        fibs     = tmp;
        fibs[fib_cnt] = next;
        fib_cnt++;
    }
}

/* Return the number of 1's in the Zeckendorf representation of n^2.
   This works for any integer type seq_t and idx_t supplied by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Compute n^2 using seq_t arithmetic.  The cast makes sure we use
       the larger type for the multiplication. */
    seq_t sq = (seq_t)n * (seq_t)n;

    /* Ensure we have Fibonacci numbers up to at least 'sq'. */
    ensure_fib_upto(sq);

    seq_t rem   = sq;
    seq_t count = 0;

    /* Greedy Zeckendorf decomposition: pick the largest Fibonacci
       number not exceeding the remainder, subtract it, and skip the
       next Fibonacci number to avoid adjacency. */
    for (size_t i = fib_cnt; i > 0; )
    {
        --i;                     /* index of current Fibonacci number */
        if (fibs[i] <= rem)
        {
            rem -= fibs[i];
            ++count;
            if (rem == 0) break;
            if (i == 0) break;  /* no lower number to skip */
            --i;                /* skip the next Fibonacci number */
        }
    }

    return count;
}
