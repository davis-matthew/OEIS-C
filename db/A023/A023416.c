/*
 * A023416  Number of 0's in binary expansion of n.
 * https://oeis.org/A023416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy, base
 * Formula:   a(n) = number of 0's in the binary expansion of n (a(0)=1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;  /* binary representation of 0 is "0" */
    seq_t cnt = 0;
    while (n) {
        if ((n & 1) == 0)
            cnt++;
        n >>= 1;
    }
    return cnt;
}
