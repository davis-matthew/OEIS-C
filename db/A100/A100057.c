/*
 * A100057  Sum of absolute differences of p(n) defined in A054065, oriented
 *          around a clock.
 * https://oeis.org/A100057
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is computed by summing blocks of 2·Fibonacci(k)
 *            differences. Each block k (k ≥ 2) has length F_k and contributes
 *            F_k · (2·F_k) to the sum. The first difference block starts at n
 *            = 3
 * Range:     u8 n <= 22, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* offset starts at 2 */
    if (n < 2) return (seq_t)0;
    if (n == 2) return (seq_t)2;

    idx_t remaining = n - 2;          /* number of differences to accumulate */
    seq_t sum = (seq_t)2;             /* a(2) = 2 */

    /* Fibonacci numbers: F1 = 1, F2 = 1, F3 = 2, ... */
    idx_t f_prev = 1; /* F1 */
    idx_t f_curr = 1; /* F2 */

    while (remaining > 0) {
        seq_t block_len = (seq_t)f_curr;               /* F_k  */
        seq_t diff      = (seq_t)2 * f_curr;           /* 2·F_k */

        if (remaining <= f_curr) {
            /* partial block */
            sum += (seq_t)remaining * diff;
            break;
        } else {
            /* full block */
            sum += block_len * diff;
            remaining -= f_curr;
        }

        /* advance to next Fibonacci number */
        idx_t f_next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = f_next;
    }

    return sum;
}
