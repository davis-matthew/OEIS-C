/*
 * A107016  Number of odd terms in Zeckendorf representation of n.
 * https://oeis.org/A107016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of odd Fibonacci terms in the Zeckendorf
 *            representation of n (n ≥ 1)
 * Range:     u8 n <= 278, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation keeps a static cache of Fibonacci numbers
   (1, 2, 3, 5, …) and performs a greedy decomposition of n.
   The cache is extended only as far as needed for each call,
   guaranteeing O(log n) time per evaluation and low memory use. */

#include <stdbool.h>

static seq_t fibs[95];          /* 95 is enough to cover all n ≤ 2^64 */
static idx_t fib_count = 0;     /* number of valid entries in fibs */
static bool initialized = false;

/* Extend the Fibonacci cache so that fibs[fib_count-1] > max_n.
   This function is called lazily on the first call to a(). */
static void ensure_fibs(idx_t max_n)
{
    if (!initialized) {
        fibs[0] = (seq_t)1;    /* F1 */
        fibs[1] = (seq_t)2;    /* F2 (Zeckendorf uses 1,2,3,5,...) */
        fib_count = 2;
        initialized = true;
    }
    /* Grow while the largest Fibonacci number is ≤ max_n. */
    while (fibs[fib_count - 1] <= (seq_t)max_n) {
        fibs[fib_count] = fibs[fib_count - 1] + fibs[fib_count - 2];
        fib_count++;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* sequence is 1‑based; return 0 safely */

    ensure_fibs(n);

    seq_t rem   = (seq_t)n;
    seq_t count = (seq_t)0;

    /* Greedy Zeckendorf decomposition: subtract largest possible
       Fibonacci term until rem becomes 0. Count only odd terms. */
    for (idx_t i = fib_count; i > 0; --i) {
        idx_t idx = i - 1;
        if (fibs[idx] <= rem) {
            rem -= fibs[idx];
            if ((fibs[idx] & (seq_t)1) != 0) {
                count += (seq_t)1;
            }
        }
    }
    return count;
}
