/*
 * A035517  Triangular array read by rows, formed from Zeckendorf expansion of
 *          integers: repeatedly subtract the largest Fibonacci number you can
 *          until nothing remains. Row n give Z. expansion of n.
 * https://oeis.org/A035517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabf, nice, look
 * Formula:   not stated
 * Range:     u8 n <= 1308, u16 n <= 4005, u32 n <= 4005, u64 n <= 4005,
 *            u128 n <= 4005, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A035517 – Flattened Zeckendorf expansion
   For each integer m ≥ 0, compute the Zeckendorf
   expansion of m (sum of non‑consecutive Fibonacci
   numbers 1,2,3,5,8,…).  Append the terms of each row
   in ascending order, then read the resulting triangle
   row by row.  a(n) is the n‑th term in this flattened
   array (a(0)=0).  */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Special case: the first term is 0. */
    if (n == 0)
        return (seq_t)0;

    idx_t counter = 1;  /* index of the next term to output; 1 after the leading 0 */

    /* Build a small Fibonacci table up to n.  For n ≤ 10 000
       the table needs at most 20 numbers. */
    idx_t fib[32];
    size_t fib_len = 2;
    fib[0] = 1;
    fib[1] = 2;
    while (fib_len < 32) {
        idx_t next = fib[fib_len - 2] + fib[fib_len - 1];
        if (next > n)
            break;
        fib[fib_len++] = next;
    }

    /* Generate rows m = 1, 2, … until the desired index is reached. */
    for (idx_t m = 1; counter <= n; ++m) {
        /* Zeckendorf decomposition of m (descending).
           The maximum number of terms is < 32 for all m ≤ n. */
        idx_t rem = m;
        seq_t temp[32];
        size_t cnt = 0;
        while (rem > 0) {
            /* Find largest Fibonacci number ≤ rem (linear search). */
            size_t j = 0;
            for (size_t k = 0; k < fib_len; ++k)
                if (fib[k] <= rem)
                    j = k;
            temp[cnt++] = (seq_t)fib[j];
            rem -= fib[j];
        }

        /* Emit the terms in ascending order; stop when n is reached. */
        for (size_t k = 0; k < cnt; ++k) {
            if (counter == n)   /* desired term found */
                return temp[cnt - 1 - k];
            ++counter;
        }
    }

    /* The function always returns before reaching this point. */
    return (seq_t)0;
}
