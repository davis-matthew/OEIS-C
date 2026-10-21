/*
 * A067046  a(n) = lcm(n, n+1, n+2)/6.
 * https://oeis.org/A067046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+2,3)*(3-(-1)^n)/4
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t v = (seq_t)n * (seq_t)(n + 1);
    v *= (seq_t)(n + 2);
    if (n & 1U)          /* n is odd: (3-(-1)^n)=4 → v/6 */
        return v / 6;
    else                 /* n is even: (3-(-1)^n)=2 → v/12 */
        return v / 12;
}
