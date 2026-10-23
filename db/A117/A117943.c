/*
 * A117943  a(1) = 0, a(2) = 1; a(3n) = a(n); if every third term (a(3), a(6),
 *          a(9), ...) is deleted, this gives back the original sequence.
 * https://oeis.org/A117943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=0, a(2)=1; * a(3n)=a(n); * a(3n+1)=a(2n+1); *
 *            a(3n+2)=a(2n+2); * Equivalently: a(n)=a(n/3) if n%3==0;
 *            otherwise a(n)=a(n-n/3)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases.  For any larger n we iteratively reduce n until it
     * becomes 1 or 2.  The sequence values are only 0 or 1. */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    while (n > 2) {
        if (n % 3 == 0) {
            n /= 3;
        } else {
            n -= n / 3;   /* n = n - floor(n/3) */
        }
    }

    return (seq_t)(n - 1); /* 1 -> 0, 2 -> 1 */
}
