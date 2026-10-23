/*
 * A037484  Base 7 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,2.
 * https://oeis.org/A037484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=9, a(3)=64,
   a(n)=7*a(n-1)+a(n-2)-7*a(n-3) for n>=4 */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)9;
    if (n == 3) return (seq_t)64;

    seq_t nm3 = (seq_t)1; /* a(n-3) */
    seq_t nm2 = (seq_t)9; /* a(n-2) */
    seq_t nm1 = (seq_t)64;/* a(n-1) */
    seq_t next;

    for (idx_t i = 4; i <= n; ++i) {
        next = (seq_t)7 * nm1 + nm2;
        next -= (seq_t)7 * nm3;
        nm3 = nm2;
        nm2 = nm1;
        nm1 = next;
    }
    return nm1;
}
