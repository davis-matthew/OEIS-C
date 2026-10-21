/*
 * A212960  Number of (w,x,y) with all terms in {0,...,n} and |w-x| != |x-y|.
 * https://oeis.org/A212960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^3 + 6*n^2 + 4*n + 1 - (-1)^n)/4
 * Range:     u8 n <= 3, u16 n <= 24, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Promote n to the arithmetic type seq_t */
    seq_t nn   = (seq_t)n;
    seq_t n2   = nn * nn;                 /* n^2   */
    seq_t n3   = n2 * nn;                 /* n^3   */

    /* Compute the polynomial 4*n^3 + 6*n^2 + 4*n + 1 */
    seq_t val   = 4*n3 + 6*n2 + 4*nn + 1;

    /* Adjust for the (-1)^n term:
       if n is odd, (-1)^n = -1  →  subtract(-1) = +1  →  val += 1
       if n is even,(-1)^n = +1  →  subtract(+1) = -1  →  val -= 1   */
    if (n & 1U) {
        val += 1;
    } else {
        val -= 1;
    }

    /* The result is always divisible by 4, so division is exact */
    return val / 4;
}
