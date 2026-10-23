/*
 * A176355  Periodic sequence: Repeat 6, 1.
 * https://oeis.org/A176355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  cofr, cons, easy, nonn, mult
 * Formula:   a(n) = (7 + 5*(-1)^n)/2
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodic sequence: Repeat 6, 1. a(0)=6, a(1)=1. */
seq_t a(idx_t n)
{
    /* n even → 6, n odd → 1 */
    return (seq_t)((n & 1) ? 1 : 6);
}
