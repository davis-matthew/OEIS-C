/*
 * A216176  Inner product of the Zeckendorf binary representation of n and its
 *          reverse.
 * https://oeis.org/A216176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   inner product of the Zeckendorf binary representation of n and
 *            its reverse. a(n) = count of indices i (1 ≤ i ≤ L) such that
 *            bit_i(n) = 1 and bit_{L+1-i}(n) = 1, where L is the largest
 *            index with F_L ≤ n (Fibonacci numbers F_1 = 1, F_2 = 2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

enum { MAX_FIB = 94 };           /* F_93 > 2^64, so 94 is safe for all 64‑bit n */

seq_t a(idx_t n)
{
    if (n == 0)                     /* offset 1; undefined below */
        return (seq_t)0;

    /* Fibonacci numbers F_1 = 1, F_2 = 2 */
    idx_t fib[MAX_FIB];
    fib[1] = 1;
    fib[2] = 2;
    size_t len = 2;
    while (len + 1 < MAX_FIB && fib[len] <= n) {
        ++len;
        fib[len] = fib[len - 1] + fib[len - 2];
    }
    if (fib[len] > n)              /* last one may exceed n */
        --len;

    /* Zeckendorf representation */
    unsigned char bits[MAX_FIB];
    for (size_t i = 0; i < MAX_FIB; ++i)
        bits[i] = 0;
    idx_t rem = n;
    for (size_t i = len; i >= 1; --i) {
        if (fib[i] <= rem) {
            bits[i] = 1;
            rem -= fib[i];
        }
        if (i == 1) break;         /* avoid size_t underflow */
    }

    /* Inner product with reverse */
    seq_t sum = 0;
    for (size_t i = 1; i <= len; ++i) {
        if (bits[i] && bits[len + 1 - i])
            ++sum;
    }

    return sum;
}
