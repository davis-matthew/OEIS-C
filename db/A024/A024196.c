/*
 * A024196  a(n) = 2nd elementary symmetric function of the first n+1 odd
 *          positive integers.
 * https://oeis.org/A024196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(3*n^2+5*n+1)/6
 * Range:     u8 n <= 4, u16 n <= 18, u32 n <= 303, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n >= 1 */
    seq_t a1 = (seq_t)n;
    seq_t b1 = a1 + 1;
    seq_t c1 = 3 * a1 * a1 + 5 * a1 + 1;

    /* divide by 2 */
    if ((a1 & 1) == 0) {
        a1 /= 2;
    } else if ((b1 & 1) == 0) {
        b1 /= 2;
    } else {
        /* c1 is odd for all n, but guard */
        c1 /= 2;
    }

    /* divide by 3 */
    if (a1 % 3 == 0) {
        a1 /= 3;
    } else if (b1 % 3 == 0) {
        b1 /= 3;
    } else {
        c1 /= 3; /* c1 divisible by 3 iff n % 3 == 1 */
    }

    return a1 * b1 * c1;
}
