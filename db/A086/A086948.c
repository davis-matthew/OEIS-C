/*
 * A086948  a(n) = k where R(k+8) = 2.
 * https://oeis.org/A086948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 2*(10^n - 4)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n ≥ 1 in the OEIS sense. For safety, trap n == 0. */
    if (n == 0)
        return (seq_t)0;

    seq_t p = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        p *= (seq_t)10;   /* compute 10^n modulo the width of seq_t */
    }
    p = p * (seq_t)2 - (seq_t)8;   /* 2*10^n - 8  ⇔ 2*(10^n - 4) */
    return p;
}
