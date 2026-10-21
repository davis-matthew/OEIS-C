/*
 * A212278  Number of adjacent pairs of zeros (possibly overlapping) in the
 *          representation of n in base of Fibonacci numbers (A014417).
 * https://oeis.org/A212278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9498, u16 n <= 9498, u32 n <= 9498, u64 n <= 9498,
 *            u128 n <= 9498, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of adjacent zero pairs in Zeckendorf (Fibonacci base) representation */
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Fibonacci numbers used in Zeckendorf representation.
       fib[0] = 1, fib[1] = 2, fib[i] = fib[i-1] + fib[i-2] */
    static idx_t fib[100];
    static int fib_len = 0;

    if (fib_len == 0) {
        fib[0] = (idx_t)1;
        fib[1] = (idx_t)2;
        fib_len = 2;
    }

    /* Grow the Fibonacci table until it exceeds n */
    while (fib_len < 100 && fib[fib_len - 1] <= n) {
        fib[fib_len] = fib[fib_len - 1] + fib[fib_len - 2];
        ++fib_len;
    }

    idx_t rem = n;
    seq_t count = 0;
    int prev_zero = -1;   /* -1: no previous bit, 0: zero, 1: one */

    for (int i = fib_len - 1; i >= 0; --i) {
        int bit = (fib[i] <= rem) ? 1 : 0;
        if (bit) rem -= fib[i];

        if (prev_zero == 0 && bit == 0)
            count += (seq_t)1;

        prev_zero = bit;
    }

    return count;
}
