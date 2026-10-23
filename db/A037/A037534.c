/*
 * A037534  Base 9 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,1,2.
 * https://oeis.org/A037534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the base-9 number whose first n digits are the repeating
 *            * sequence 1,1,2,1,1,2,… (i.e. digits with period 3: 1,1,2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    for (idx_t i = 0; i < n; ++i) {
        /* digit pattern 1,1,2 repeats every 3 positions */
        int d = (i % 3 == 2) ? 2 : 1;
        res = res * 9 + d;
    }
    return res;
}
