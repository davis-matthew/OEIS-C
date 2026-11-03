/*
 * A031923  Let r and s be consecutive Fibonacci numbers. Sequence is r^4, r^3
 *          s, r^2 s^2, and r s^3.
 * https://oeis.org/A031923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(n) = F(floor((n-1)/4)+3) ^ ((n-1) mod 4) * *
 *            F(floor((n-1)/4)+2) ^ (4 - ((n-1) mod 4)) * where F(m) is the
 *            m-th Fibonacci number (F(1)=1, F(2)=1)
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 45, u64 n <= 91, u128 n <= 184,
 *            big any n
 * Validated: standard, pass, 54 terms through n = 999 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t fib(idx_t n) {
    if (n <= 2) return (seq_t)1;
    seq_t f1 = 1, f2 = 1, f3;
    for (idx_t i = 3; i <= n; ++i) {
        f3 = f1 + f2;
        f1 = f2;
        f2 = f3;
    }
    return f2;
}

seq_t a(idx_t n)
{
    /* Determine the block index and Fibonacci indices */
    idx_t k = (n - 1) / 4;          /* block number (0‑based) */
    idx_t r_idx = k + 3;            /* larger Fibonacci index */
    idx_t s_idx = k + 2;            /* smaller Fibonacci index */

    /* Fetch the two consecutive Fibonacci numbers */
    seq_t r = fib(r_idx);
    seq_t s = fib(s_idx);

    /* Exponents derived from the position within the block */
    int exp_r = (int)((n - 1) % 4); /* 0..3 */
    int exp_s = 4 - exp_r;          /* 1..4 */

    /* Compute the power product */
    seq_t result = 1;
    for (int i = 0; i < exp_r; ++i) result *= r;
    for (int i = 0; i < exp_s; ++i) result *= s;

    return result;
}
