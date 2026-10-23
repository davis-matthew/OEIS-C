/*
 * A197353  a(0)=0, a(1)=1, a(2n)=19*a(n), a(2n+1)=a(2n)+1.
 * https://oeis.org/A197353
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k>=0} A030308(n,k)*19^k (i.e. sum over the binary
 *            digits of n, each bit multiplied by 19^position)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;   /* the result */
    seq_t p   = 1;   /* current power of 19 (19^k) */

    idx_t m = n;     /* working copy of the index */
    while (m) {
        if (m & 1)   /* if the lowest bit is set, add this power of 19 */
            sum += p;
        p *= 19;     /* move to the next power of 19 */
        m >>= 1;     /* shift n right to process the next bit */
    }
    return sum;
}
