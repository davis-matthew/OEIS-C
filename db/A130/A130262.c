/*
 * A130262  Partial sums of the 'upper' even Fibonacci Inverse A130260.
 * https://oeis.org/A130262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*A130260(n) - Fib(2*A130260(n)-1) + 1
 * Range:     u8 n <= 57, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes a(n) by summing blocks of equal differences.
   Each integer j repeats Fib(2*j-1) times, where the block contributes j
   to the running sum. */
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;          /* work with a seq_t copy for comparisons */
    seq_t total_counts = 0;      /* cumulative number of terms processed */
    seq_t total_value   = 0;      /* accumulated a(n) up to the last full block */
    idx_t j = 1;                 /* current block value */

    while (1) {
        /* number of occurrences of the current block value j */
        seq_t repeat = fib(2 * j - 1);

        /* if adding this whole block would exceed or reach n terms,
           take only the needed amount and return the result */
        if (total_counts + repeat >= N) {
            seq_t needed = N - total_counts;  /* how many of j's we need */
            total_value += needed * (seq_t)j;
            return total_value;
        }

        /* otherwise consume the whole block and continue */
        total_counts += repeat;
        total_value  += repeat * (seq_t)j;
        ++j;
    }
}
