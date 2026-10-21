/*
 * A216225  Distance between n^2 and next higher Fibonacci number.
 * https://oeis.org/A216225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = fib(x) - n^2, where fib(x) ≥ n^2 and fib(x‑1) < n^2;
 *            returns 0 if n^2 is a Fibonacci number
 * Range:     u8 n <= 12, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Compute n*n in seq_t to avoid overflow when n is large. */
    seq_t nsq = (seq_t)n * (seq_t)n;

    /* Special case: 0 */
    if (nsq == 0) {
        return (seq_t)0;
    }

    /* Find an upper bound for the Fibonacci index whose value is ≥ nsq. */
    idx_t upper = 1;
    while (fib(upper) < nsq) {
        upper <<= 1;
    }

    /* Binary search for the smallest index with fib(index) ≥ nsq. */
    idx_t low = 0;
    idx_t high = upper;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (fib(mid) >= nsq) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }

    /* low is the desired index */
    seq_t f_val = fib(low);
    return f_val - nsq;  /* Difference is zero when n^2 is itself a Fibonacci number. */
}
