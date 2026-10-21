/*
 * A098871  Sums of distinct powers of 4 plus 1.
 * https://oeis.org/A098871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000695(n)+1, where A000695(n) = sum_{i} ( (n>>i)&1 ) *
 *            4^i
 * Range:     u8 n <= 15, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t value = 0;          /* Accumulator for A000695(n)          */
    seq_t power = ((seq_t)1); /* 4^0 as integer (shifted by 0 bits)   */
    idx_t bit = n;            /* Local copy to iterate over bits      */
    while(bit)
    {
        if(bit & 1)
            value += power;
        power <<= 2;  /* multiply by 4: 1 << (2*next_bitpos) */
        bit >>= 1;
    }
    return value + 1;           /* add the constant 1 as per definition */
}
