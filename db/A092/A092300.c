/*
 * A092300  a(n) = 2*5^(n^2+2n+1)*Product_{j=1..n} (25^j-1).
 * https://oeis.org/A092300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*5^(n^2+2n+1) * Product_{j=1..n} (25^j-1)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)10;        /* a(0) = 10 */

    seq_t val = (seq_t)10;                /* current a(i-1) */
    seq_t p5  = (seq_t)1;                 /* 5^(2*(i-1)) */

    for (idx_t i = 1; i <= n; ++i) {
        p5 = p5 * (seq_t)25;             /* p5 = 5^(2*i) */

        /* factor = 5 * p5 * (p5 - 1)   (p5 = 5^(2*i)) */
        seq_t factor = (seq_t)5 * p5 * (p5 - (seq_t)1);

        val = val * factor;              /* a(i) = a(i-1) * factor */
    }
    return val;
}
