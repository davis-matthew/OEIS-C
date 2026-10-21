/*
 * A221905  a(n) = 3^n + 3*n.
 * https://oeis.org/A221905
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n + 3*n
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow3 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow3 *= (seq_t)3;            /* raise 3 to the n‑th power */

    seq_t addend = (seq_t)3 * (seq_t)n;  /* compute 3*n */
    return pow3 + addend;               /* final result */
}
