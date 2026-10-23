/*
 * A037792  Decimal expansion of a(n) is given by the first n terms of the
 *          periodic sequence with initial period 3,2,0,1.
 * https://oeis.org/A037792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n‑digit number whose decimal representation consists
 *            of the first n terms of the periodic sequence 3,2,0,1. a(1)=3,
 *            a(2)=32, a(3)=320, …
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const unsigned char period[4] = {3, 2, 0, 1};
    seq_t result = 0;

    /* Build the decimal expansion digit by digit. */
    for (idx_t i = 0; i < n; ++i)
    {
        /* Align the index with the period and convert the digit to seq_t. */
        result = result * (seq_t)10 + (seq_t)period[i & 3U];
    }

    return result;
}
