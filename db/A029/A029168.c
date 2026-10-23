/*
 * A029168  Expansion of 1/((1-x^2)*(1-x^3)*(1-x^10)*(1-x^11)).
 * https://oeis.org/A029168
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((131*n^3+69*n^2+(78-9*(n mod 2))*n+432)/360) -
 *            floor((4*n^3+2*n^2+n+8)/11)
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast idx_t to seq_t for arithmetic */
    seq_t ns  = (seq_t)n;
    seq_t n2  = ns * ns;          /* n^2 */
    seq_t n3  = n2 * ns;          /* n^3 */

    /* first floor term */
    seq_t t1 = (seq_t)131 * n3
             + (seq_t)69  * n2
             + (((seq_t)78 - (seq_t)9 * (n & 1)) * ns)
             + (seq_t)432;
    seq_t part1 = t1 / (seq_t)360;

    /* second floor term */
    seq_t t2 = (seq_t)4 * n3
             + (seq_t)2 * n2
             + ns
             + (seq_t)8;
    seq_t part2 = t2 / (seq_t)11;

    return part1 - part2;
}
