/*
 * A219730  Sum_{x <= n} smallest divisor of x that is >= sqrt(x).
 * https://oeis.org/A219730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{x=1..n} smallest divisor of x that is >= sqrt(x)
 * Range:     u8 n <= 30, u16 n <= 659, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Integer floor square root: returns ⌊√n⌋ for a 64‑bit unsigned integer n. */
static inline idx_t floor_isqrt(idx_t n) {
    idx_t x = n;
    idx_t y = (x + 1) >> 1;          /* initial guess */
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;        /* Newton step */
    }
    return x;
}

/* Computes a(n) for OEIS A219730.  The function may be called in any order. */
seq_t a(idx_t n) {
    if (n < 1)
        return 0;
    seq_t sum = 0;
    for (idx_t x = 1; x <= n; ++x) {
        idx_t s = floor_isqrt(x);
        if (s == 0)          /* safety for x = 0, not used in this loop */
            s = 1;
        idx_t d = x;          /* default for primes: use x itself */
        for (idx_t i = s; i >= 1; --i) {
            if (x % i == 0) {
                d = x / i;    /* smallest divisor >= sqrt(x) */
                break;
            }
        }
        sum += (seq_t)d;
    }
    return sum;
}
