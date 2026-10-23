/*
 * A192328  Numbers of the form 20*k+7 which are three times a square.
 * https://oeis.org/A192328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3 * ((10*(n-1)+(-1)^(n-1)+5)/2)^2
 * Range:     u8 n <= 2, u16 n <= 30, u32 n <= 7568, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent simplified form:  a(n) = 3 * (5*n - (n is odd?2:3))^2   */

seq_t a(idx_t n)
{
    /* n >= 1 (offset is 1).  Any n < 1 is not required. */
    seq_t sn = (seq_t)n;
    seq_t t;

    if ((n & 1ULL) == 0) {          /* n is even: (-1)^(n-1) = -1   */
        /* t = (10*n - 10 + (-1) + 5)/2 = (10*n - 6)/2 = 5*n - 3 */
        t = sn * (seq_t)5 - (seq_t)3;
    } else {                        /* n is odd: (-1)^(n-1) = 1    */
        /* t = (10*n - 10 + 1 + 5)/2 = (10*n - 4)/2 = 5*n - 2  */
        t = sn * (seq_t)5 - (seq_t)2;
    }

    /* a(n) = 3 * t * t  */
    return (seq_t)3 * t * t;
}
