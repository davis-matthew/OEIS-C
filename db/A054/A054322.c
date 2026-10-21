/*
 * A054322  Fourth unsigned column of Lanczos triangle A053125 (decreasing
 *          powers).
 * https://oeis.org/A054322
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4^n * binomial(2*n+4, 3) (offset 0)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 24, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the term a(n) of OEIS sequence A054322. */
seq_t a(idx_t n)
{
    /* 4^n computed by exponentiation by squaring to keep the number of
       multiplications low (O(log n)).  For large n this also avoids
       building an intermediate pow4 that overflows in 64‑bit when the
       result itself also overflows. */
    seq_t pow4 = 1;
    seq_t base  = 4;   /* 4^1 */
    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) pow4 *= base;
        base *= base;
        exp >>= 1;
    }

    /* Binomial coefficient binomial(2n+4,3) = (2n+4)(2n+3)(2n+2)/6.
       The three numerators are at most about 4n, which for the test
       ranges fits comfortably in seq_t even for 64‑bit builds. */
    seq_t t1 = (seq_t)(2 * n + 2); /* 2n + 2 */
    seq_t t2 = (seq_t)(2 * n + 3); /* 2n + 3 */
    seq_t t3 = (seq_t)(2 * n + 4); /* 2n + 4 */

    seq_t binom = t1 * t2 * t3 / 6;   /* exact division by 6 */

    /* Final term */
    return pow4 * binom;
}
