/*
 * A033052  a(1) = 1, a(2n) = 16a(n), a(2n+1) = a(2n)+1.
 * https://oeis.org/A033052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0)=0, a(1)=1, a(2n)=16*a(n), a(2n+1)=a(2n)+1 Equivalent to
 *            a(n) = sum_{i} (n>>i & 1) * 16^i
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t base   = 1;          /* 16^i, i starts at 0 */
    idx_t temp   = n;          /* work on a copy of n */

    /* iterate over all bits of n */
    while (temp)
    {
        if (temp & 1U)
            result += base;   /* add 16^i if the bit is set */

        /* prepare for next bit: base *= 16 (shift left by 4) */
        base <<= 4;

        /* advance to next bit */
        temp >>= 1;
    }

    return result;
}
