/*
 * A188067  Expansion of x^2*(x^3+2*x^2+x+1)/((x-1)*(x+1))^4.
 * https://oeis.org/A188067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(n+2)/16 if n is even a(n) = n*(n^2-1)/24 if n is odd
 *            (which is equivalent to a(n) = n*(
 *            n*(5*n+6)+(n^2+6*n+2)*(-1)^n-2 ) / 96)
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n & 1) {                /* odd n */
        seq_t tn = (seq_t)n;
        seq_t t  = tn * tn;     /* n^2 */
        t = t - 1;              /* n^2 - 1 */
        t = t * tn;             /* n * (n^2 - 1) */
        return t / 24;
    } else {                    /* even n */
        seq_t tn = (seq_t)n;
        seq_t t  = tn * tn;     /* n^2 */
        t = t * (tn + 2);       /* n^2 * (n + 2) */
        return t / 16;
    }
}
