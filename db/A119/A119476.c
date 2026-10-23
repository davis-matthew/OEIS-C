/*
 * A119476  a(1)=1, a(n)=a((n+1)/2)+1 if n is odd, a(n)=a(n/2)+2 if n is even.
 * https://oeis.org/A119476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; a(n)=a((n+1)/2)+1 if n odd; a(n)=a(n/2)+2 if n even
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    while (n > 1) {
        if (n & 1) {          /* n is odd: a((n+1)/2)+1 */
            n = (n >> 1) + 1; /* (n+1)/2 == (n>>1)+1 for odd n */
            result += 1;
        } else {              /* n is even: a(n/2)+2 */
            n >>= 1;          /* n/2 */
            result += 2;
        }
    }
    /* base case a(1) = 1 */
    return result + 1;
}
