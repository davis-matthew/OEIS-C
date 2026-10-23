/*
 * A037535  Decimal expansion of a(n) is given by the first n terms of the
 *          periodic sequence with initial period 1,1,2.
 * https://oeis.org/A037535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor((112/999)*10^n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) as the integer formed by the first n digits of the periodic sequence 1,1,2,1,1,2,... */
seq_t a(idx_t n)
{
    seq_t val = 0;
    for (idx_t i = 1; i <= n; ++i) {
        val *= 10;              /* shift left by one decimal digit */
        if ((i % 3) == 0)       /* digit 2 at positions 3,6,9,... */
            val += 2;
        else                    /* digits 1 at all other positions */
            val += 1;
    }
    return val;
}
