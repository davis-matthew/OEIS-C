/*
 * A069705  a(n) = 2^n mod 7.
 * https://oeis.org/A069705
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n mod 7
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Periodicity modulo 7 gives the repeating pattern 1,2,4. */
    static const seq_t tbl[3] = {(seq_t)1, (seq_t)2, (seq_t)4};
    return tbl[n % 3];
}
