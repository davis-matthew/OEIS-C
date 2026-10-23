/*
 * A046636  Number of cubic residues mod 8^n.
 * https://oeis.org/A046636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*8^n + 3)/7
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1, a(n) = 8*a(n-1) - 3 */
seq_t a(idx_t n)
{
    seq_t v = 1;                 /* a(0) */
    for (idx_t i = 0; i < n; ++i) {
        v = 8 * v - 3;           /* a(i+1) = 8*a(i) - 3 */
    }
    return v;
}
