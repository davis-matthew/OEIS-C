/*
 * A056974  Number of blocks of {0, 0, 0} in the binary expansion of n.
 * https://oeis.org/A056974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = # of overlapping blocks of 000 in the binary expansion of
 *            n. For offset 1 the recurrence is a(1)=0, a(2n)=a(n)+[n≡0 mod
 *            8], a(2n+1)=a(n). The function below computes the value directly
 *            by scanning the bits of n
 * Range:     u8 n <= 9705, u16 n <= 9705, u32 n <= 9705, u64 n <= 9705,
 *            u128 n <= 9705, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t x = n;
    while (x != 0)
    {
        /* if the lowest three bits are zero, we found a block */
        if ((x & 7U) == 0U)
            ++count;
        x >>= 1;
    }
    return count;
}
