/*
 * A233137  Reversed shortest (x+1,2x)-code of n.
 * https://oeis.org/A233137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 94, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A233137:  Reversed shortest (x+1,2x)-code of n
 *
 * For n = 1, the value is 1.
 * For n > 1, repeatedly apply:
 *   - if n is even, set n = n/2 and record digit 2
 *   - if n is odd, set n = n-1 and record digit 1
 * Concatenate the recorded digits (in the order they are produced) to form
 * a decimal number; that number is the sequence value a(n).
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;   /* a(1) = 1 */

    seq_t result = 0;
    idx_t m = n;

    while (m > 1) {
        if ((m & 1) == 0) {          /* even: halve */
            result = result * 10 + 2;
            m >>= 1;                 /* m = m / 2 */
        } else {                     /* odd: decrement */
            result = result * 10 + 1;
            m--;                     /* m = m - 1 */
        }
    }
    return result;
}
