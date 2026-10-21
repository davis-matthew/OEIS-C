/*
 * A213396  Number of (w,x,y) with all terms in {0,...,n} and 2*w < |x+y-w|.
 * https://oeis.org/A213396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+2)*(n+1)*n/3 + floor((n-1)/3) + 1
 * Range:     u8 n <= 8, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }

    seq_t a = n;
    seq_t b = n + 1;
    seq_t c = n + 2;

    /* Divide one of a, b, c by 3 (among consecutive ints one is divisible by 3) */
    if (a % 3 == 0) {
        a /= 3;
    } else if (b % 3 == 0) {
        b /= 3;
    } else {
        c /= 3;
    }

    seq_t term1 = a * b * c;          /* (n+2)*(n+1)*n / 3  */
    seq_t term2 = (n - 1) / 3;        /* floor((n-1)/3)     */

    return term1 + term2 + 1;
}
