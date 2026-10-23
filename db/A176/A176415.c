/*
 * A176415  Periodic sequence: repeat (7,1).
 * https://oeis.org/A176415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  cofr, cons, easy, nonn, mult
 * Formula:   a(n) = 4 + 3*(-1)^n
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Periodic sequence with values 7, 1, 7, 1, ... */
seq_t a(idx_t n)
{
    return (seq_t)((n & 1) ? 1ULL : 7ULL);
}
