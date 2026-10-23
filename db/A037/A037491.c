/*
 * A037491  Base 6 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,1.
 * https://oeis.org/A037491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor(13*6^n/35)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* fast exponentiation of 6^n */
    seq_t pow = 1;
    seq_t base = 6;
    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1)
            pow = pow * base;
        base = base * base;
        exp >>= 1;
    }
    /* compute floor(13*pow/35) */
    return ( (seq_t)13 * pow ) / ((seq_t)35);
}
