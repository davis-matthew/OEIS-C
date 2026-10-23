/*
 * A102364  Number of terms in Fibonacci sequence less than n not used in
 *          Zeckendorf representation of n (the Zeckendorf representation of n
 *          is a sum of non-consecutive distinct Fibonacci numbers).
 * https://oeis.org/A102364
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9712, u16 n <= 9712, u32 n <= 9712, u64 n <= 9712,
 *            u128 n <= 9712, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of Fibonacci numbers <= n that are NOT used in the Zeckendorf
   representation of n (the greedy algorithm with Fibonacci numbers 1,2,3,5,…).
   Computation: generate all Fibonacci numbers <= n, then perform the greedy
   subtraction to count how many are used.  The answer is the total number
   minus the used count. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the trivial cases fast. */
    if (n == 0)
        return 0;

    /* Fibonacci numbers in the Zeckendorf system start with 1,2. */
    idx_t fib[93];             /* enough for 64‑bit Fibonacci limits */
    idx_t fibcnt = 0;

    idx_t f1 = 1;
    idx_t f2 = 2;

    /* Include the first Fibonacci number if it does not exceed n. */
    if (n >= 1)
        fib[fibcnt++] = 1;     /* F(1) = 1 */

    /* Generate subsequent Fibonacci numbers while they are <= n. */
    while (f2 <= n && fibcnt < (idx_t)93) {
        fib[fibcnt++] = f2;    /* F(2), F(3), … */
        idx_t next = f1 + f2;
        f1 = f2;
        f2 = next;
    }

    /* Greedy Zeckendorf decomposition: subtract the largest possible
       Fibonacci numbers first. */
    idx_t rem = n;
    idx_t used = 0;
    for (idx_t i = fibcnt; i-- > 0;) {
        if (rem >= fib[i]) {
            rem -= fib[i];
            used++;
        }
    }

    /* Count zeros = total Fibonacci numbers up to n minus those used. */
    return (seq_t)(fibcnt - used);
}
