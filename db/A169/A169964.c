/*
 * A169964  Numbers whose decimal expansion contains only 0's and 5's.
 * https://oeis.org/A169964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 5 * A007088(n-1) (A007088 lists numbers with digits 0/1)
 * Range:     u8 n <= 4, u16 n <= 32, u32 n <= 512, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) = 0 is the base case */
    if (n <= 1) 
        return 0;

    /* Convert n-1 from binary to a decimal number whose digits are 0/1 */
    idx_t k   = n - 1;
    seq_t dec = 0;

    /* Determine the number of bits in k */
    idx_t msb = 0;
    idx_t tmp = k;
    while (tmp) {
        msb++;
        tmp >>= 1;
    }

    /* Process bits from most significant to least significant */
    for (idx_t i = msb - 1; ; i--) {
        idx_t bit = (k >> i) & 1u;
        dec = dec * 10 + (seq_t)bit;
        if (i == 0) 
            break;
    }

    /* Replace 1 with 5 by multiplying by 5 */
    return dec * 5;
}
