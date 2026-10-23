/*
 * A105395  A simple "Fractal Jump Sequence" (FJS).
 * https://oeis.org/A105395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; a(2)=1; a(3)=1; a(4)=7; a(n)=a(n-3) for n>4. */
seq_t a(idx_t n)
{
    if (n <= 3) {
        return (seq_t)1;
    }
    if (n == 4) {
        return (seq_t)7;
    }
    return ((n % 3) == 1) ? (seq_t)7 : (seq_t)1;
}
