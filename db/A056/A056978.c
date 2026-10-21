/*
 * A056978  Number of blocks of {1, 0, 0} in binary expansion of n.
 * https://oeis.org/A056978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of occurrences of binary pattern "100" in the
 *            binary expansion of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;          /* number of "100" blocks found */
    idx_t temp = n;           /* temporary copy of n for shifting */

    /* Check each 3-bit window by shifting right one bit each time.
       The loop stops when all remaining bits are fewer than 3 (i.e. < 4). */
    while (temp >= 4UL)
    {
        if ((temp & 7ULL) == 4ULL)   /* 7 = 111b, 4 = 100b */
            count++;
        temp >>= 1;
    }
    return count;
}
