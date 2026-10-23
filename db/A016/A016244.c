/*
 * A016244  Expansion of 1/((1-x)*(1-6*x)*(1-9*x)).
 * https://oeis.org/A016244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=16, a(n)=15*a(n-1)-54*a(n-2)+1 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)16;
    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)16;
    seq_t ai;
    for (idx_t i = 2; i <= n; ++i) {
        ai = (seq_t)15 * a1 - (seq_t)54 * a0 + (seq_t)1;
        a0 = a1;
        a1 = ai;
    }
    return a1;
}
