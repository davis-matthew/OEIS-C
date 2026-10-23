/*
 * A155626  a(n) = 5^n-4^n+1.
 * https://oeis.org/A155626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n - 4^n + 1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes power base^exp using repeated multiplication. */
static seq_t _pow_uint(seq_t base, idx_t exp)
{
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i)
        result *= base;
    return result;
}

seq_t a(idx_t n)
{
    /* 5^n */
    seq_t pow5 = _pow_uint(5, n);
    /* 4^n */
    seq_t pow4 = _pow_uint(4, n);
    /* 5^n - 4^n + 1 */
    return pow5 - pow4 + 1;
}
