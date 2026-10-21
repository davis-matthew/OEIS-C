/*
 * A072649  n occurs Fibonacci(n) times (cf. A000045).
 * https://oeis.org/A072649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = number of distinct Fibonacci numbers ≤ n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) returns the count of Fibonacci numbers not exceeding n, 
   where the Fibonacci sequence is 1,1,2,3,5,8,... and the duplicate 1 is counted only once.
   Thus a(1)=1, a(2)=2, a(3)=3, a(4)=3, a(5)=4, … */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Precompute all distinct Fibonacci numbers that fit in 64‑bit unsigned integers.
       There are at most 92 such numbers (up to F_93 = 12200160415121876738 < 2^64-1). */
    static uint64_t fibs[93];
    static int fib_count = 0;

    if (fib_count == 0) {
        /* Distinct list: 1, 2, 3, 5, 8, …  (skipping the second 1) */
        fibs[0] = 1;
        fibs[1] = 2;
        fib_count = 2;

        while (1) {
            uint64_t a = fibs[fib_count - 2];
            uint64_t b = fibs[fib_count - 1];

            /* Detect overflow before addition */
            if (UINT64_MAX - a < b)
                break;

            uint64_t c = a + b;
            fibs[fib_count++] = c;

            /* c is already <= UINT64_MAX by construction */
            if (c >= UINT64_MAX)
                break;
        }
    }

    /* Count how many Fibonacci numbers are ≤ n */
    int i = 0;
    while (i < fib_count && fibs[i] <= n)
        ++i;

    return (seq_t)i;
}
