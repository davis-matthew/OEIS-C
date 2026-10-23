/*
 * A171408  a(n) = A171373(n+1) - 2*A171373(n).
 * https://oeis.org/A171408
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 9, i16 n <= 20, i32 n <= 41, i64 n <= 41, i128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 3*a(n-1) - 4*a(n-2) + 2*a(n-3) - a(n-4)  (n ≥ 4)
 * Initial values: a(0) = 4, a(1) = 4, a(2) = 4, a(3) = 4
 */
seq_t a(idx_t n)
{
    if (n == 0 || n == 1 || n == 2 || n == 3)
        return (seq_t)4;

    seq_t a0 = (seq_t)4, a1 = (seq_t)4, a2 = (seq_t)4, a3 = (seq_t)4, ai;
    for (idx_t i = 4; i <= n; ++i) {
        ai = (seq_t)3 * a3 - (seq_t)4 * a2 + (seq_t)2 * a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }
    return ai;
}
