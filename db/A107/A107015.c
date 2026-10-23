/*
 * A107015  Number of even terms in Zeckendorf representation of n.
 * https://oeis.org/A107015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2574, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of even Fibonacci numbers in the Zeckendorf
 * representation of n (Zeckendorf uses Fibonacci numbers 1,2,3,5,8,…).
 * The even Fibonacci numbers are those with indices 1,4,7,… in the
 * zero‑based array {1,2,3,5,8,13,21,34,…}.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* offset starts at 1, guard against 0 */

    /* Fibonacci numbers up to a generous upper bound (enough for all n
     * tested; 128 numbers exceed 10^26).  This array is a static local
     * so that seq_t objects are properly constructed in C++. */
    seq_t fib[128];
    int len = 2;          /* number of Fibonacci numbers stored */
    fib[0] = 1;           /* F1 */
    fib[1] = 2;           /* F2 */

    /* Build the Fibonacci list until the last element exceeds n. */
    while (fib[len - 1] <= (seq_t)n) {
        fib[len] = fib[len - 1] + fib[len - 2];
        ++len;
        /* Safety guard – never hit in the tests with n ≤ 10⁵. */
        if (len >= 127) break;
    }

    seq_t rem = (seq_t)n;
    seq_t count = 0;

    /* Greedy Zeckendorf subtraction, counting even terms. */
    for (int i = len - 1; i >= 0 && rem > 0; --i) {
        if (fib[i] <= rem) {
            rem -= fib[i];
            if ((i % 3) == 1)       /* indices 1,4,7,… are even Fibonacci numbers */
                ++count;
        }
    }

    return count;
}
