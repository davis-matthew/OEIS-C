/*
 * A173426  a(n) is obtained by starting with 1, sequentially concatenating
 *          all decimal numbers up to n, and then, starting from n-1,
 *          sequentially concatenating all decimal numbers down to 1.
 * https://oeis.org/A173426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenate(1,2,...,n-1,n,n-1,...,2,1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute the number of decimal digits of x (x >= 1) */
static inline int digits(idx_t x) {
    int d = 0;
    do {
        x /= 10;
        d++;
    } while (x);
    return d;
}

/* Concatenate numbers 1..n then n-1..1 */
seq_t a(idx_t n) {
    /* precompute powers of 10 up to 10^18 (enough for any 64‑bit n) */
    static seq_t pow10[20];
    static bool initialized = false;
    if (!initialized) {
        pow10[0] = (seq_t)1;
        for (int i = 1; i < 20; ++i)
            pow10[i] = pow10[i - 1] * (seq_t)10;
        initialized = true;
    }

    seq_t result = (seq_t)0;

    /* 1 .. n */
    for (idx_t i = 1; i <= n; ++i) {
        int d = digits(i);
        result = result * pow10[d] + i;
    }

    /* n-1 .. 1 */
    if (n > 1) {
        for (idx_t i = n - 1; i > 0; --i) {
            int d = digits(i);
            result = result * pow10[d] + i;
        }
    }

    return result;
}
