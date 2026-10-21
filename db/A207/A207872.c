/*
 * A207872  Denominator of Z(n,1/2), where Z(n,x) is the n-th Zeckendorf
 *          polynomial.
 * https://oeis.org/A207872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 2^k where k is the smallest integer such that * F(k+3) ≥
 *            n+1, with F(m) the Fibonacci numbers (F(1)=1, F(2)=1). * Hence 1
 *            appears once, 2 once, 4 twice, 8 three times, 16 five times, etc
 * Range:     u8 n <= 54, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the smallest k so that Fibonacci(F(k+3)) >= n+1 */
    seq_t f_prev = 1;          /* F(1) */
    seq_t f_cur  = 1;          /* F(2) */
    idx_t  m     = 2;          /* current Fibonacci index i (f_cur = F(i)) */

    /* target value n+1, converted to seq_t on each comparison */
    while ((seq_t)f_cur < (seq_t)n + 1) {
        seq_t f_next = f_prev + f_cur;
        f_prev = f_cur;
        f_cur  = f_next;
        m++;
    }

    idx_t k = m - 3;   /* exponent for the denominator */

    /* Compute 2^k */
    seq_t val = 1;
    for (idx_t i = 0; i < k; ++i)
        val = val * 2;

    return val;
}
