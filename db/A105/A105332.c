/*
 * A105332  a(n) = n*(n+1)/2 mod 8.
 * https://oeis.org/A105332
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)/2 mod 8
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodic with period 16: a(n) = a(n mod 16) */
seq_t a(idx_t n)
{
    /* table of the 16 terms mod 8 */
    static const seq_t table[16] = {
        0, 1, 3, 6, 2, 7, 5, 4,
        4, 5, 7, 2, 6, 3, 1, 0
    };
    /* use the low 4 bits of n to index the table */
    return table[n & 0xFu];
}
