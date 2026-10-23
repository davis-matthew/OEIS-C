/*
 * A014710  The regular paper-folding (or dragon curve) sequence. Alphabet
 *          {2,1}.
 * https://oeis.org/A014710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(4*n) = 2 * a(4*n+2) = 1 * a(2*n+1) = a(n) * a(0) = 2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    while (1) {
        if (n == 0 || n == 1) {          /* base cases a(0)=a(1)=2 */
            return (seq_t)2;
        }
        if ((n & 1) == 0) {              /* even index */
            if ((n & 3) == 0) {          /* n % 4 == 0 -> a(4*k) = 2 */
                return (seq_t)2;
            } else {                     /* n % 4 == 2 -> a(4*k+2) = 1 */
                return (seq_t)1;
            }
        } else {                         /* odd index: a(2*k+1) = a(k) */
            n >>= 1;                     /* k = n / 2 */
        }
    }
}
