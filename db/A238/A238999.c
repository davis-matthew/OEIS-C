/*
 * A238999  Number of partitions of n using Fibonacci numbers > 1.
 * https://oeis.org/A238999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 169, u32 n <= 912, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of partitions of n using Fibonacci numbers > 1.
   Generating function: 1 / ∏_{i≥3} (1 - x^{Fib(i)})                           */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the first two offsets explicitly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    /* Dynamic programming array for all values up to n.            */
    seq_t *ways = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!ways) return (seq_t)0;                      /* out‑of‑memory */
    ways[0] = (seq_t)1;

    /* Generate Fibonacci numbers greater than 1: 2, 3, 5, 8, …       */
    idx_t f_prev = 1, f_curr = 1;                   /* fib(k-2), fib(k-1) */
    for (;;)
    {
        idx_t f = f_prev + f_curr;                  /* next Fibonacci number */
        if (f > n) break;                           /* no larger coins useful */

        /* Classic 1‑dimensional coin‑change DP using coin f.       */
        for (idx_t i = f; i <= n; ++i)
            ways[i] += ways[i - f];

        f_prev = f_curr;
        f_curr = f;
    }

    seq_t result = ways[n];
    free(ways);
    return result;
}
