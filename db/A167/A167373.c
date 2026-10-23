/*
 * A167373  Expansion of (1+x)*(3*x+1)/(1+x+x^2).
 * https://oeis.org/A167373
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=1, a(1)=3, a(2)=-1; for n>2: a(n) = -a(n-1)-a(n-2). This
 *            yields the repeating pattern for n≥1: 3, -1, -2 with period 3
 * Range:     i8 n <= 104, i16 n <= 104, i32 n <= 104, i64 n <= 104,
 *            i128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    /* n >= 1 : use the period‑3 pattern */
    switch (n % 3) {
        case 0: return (seq_t)-2; /* n % 3 == 0  => a(n) = -2 */
        case 1: return (seq_t) 3; /* n % 3 == 1  => a(n) = 3  */
        case 2: return (seq_t)-1; /* n % 3 == 2  => a(n) = -1 */
        default: return (seq_t)0; /* unreachable but keeps compiler happy */
    }
}
