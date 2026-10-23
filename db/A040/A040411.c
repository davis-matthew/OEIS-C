/*
 * A040411  Continued fraction for sqrt(432).
 * https://oeis.org/A040411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0)=20; for n>0, a(n)=1 unless 2 divides n. If n=2^e·m with m
 *            odd, a(n)=3 when e=1, a(n)=1 when e=2, and a(n)=40 when e≥3
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute multiplicative sequence A040411 */
seq_t a(idx_t n)
{
    /* Special case for the offset 0 */
    if (n == 0)
        return (seq_t)20;

    /* All odd n (and n=1) give 1 */
    if ((n & 1) != 0)
        return (seq_t)1;

    /* Count exponent of 2 in n */
    unsigned e = 0;
    idx_t m = n;
    while ((m & 1) == 0) {
        ++e;
        m >>= 1;
    }

    if (e == 1)
        return (seq_t)3;
    if (e == 2)
        return (seq_t)1;
    /* e >= 3 */
    return (seq_t)40;
}
