/*
 * A037586  Base 7 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,1.
 * https://oeis.org/A037586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 7*a(n-1) + a(n-2) - 7*a(n-3); Initial values: a(1)=3,
 *            a(2)=22, a(3)=157
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* below offset; behaviour does not matter */
    if (n == 1) return 3;
    if (n == 2) return 22;
    if (n == 3) return 157;

    seq_t a0 = 3;   /* a(n-3) */
    seq_t a1 = 22;  /* a(n-2) */
    seq_t a2 = 157; /* a(n-1) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = a2 * 7 + a1 - a0 * 7;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
