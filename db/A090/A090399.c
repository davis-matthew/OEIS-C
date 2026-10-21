/*
 * A090399  Expansion of 1/(1-2x+2x^4).
 * https://oeis.org/A090399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(0)=1, a(1)=2, a(2)=4, a(3)=8, a(n) = 2*a(n-1) - 2*a(n-4) for n
 *            ≥ 4 (expansion of 1/(1-2x+2x⁴))
 * Range:     i8 n <= 8, i16 n <= 21, i32 n <= 46, i64 n <= 97, i128 n <= 198,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the small indices directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)8;

    /* maintain the last four values of the recurrence */
    seq_t a0 = (seq_t)1, a1 = (seq_t)2, a2 = (seq_t)4, a3 = (seq_t)8;
    seq_t next;
    for (idx_t i = 4; i <= n; ++i) {
        next = (seq_t)2 * a3 - (seq_t)2 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
