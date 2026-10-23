/*
 * A159355  Number of n X n arrays of squares of integers summing to 4.
 * https://oeis.org/A159355
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n^2,4) + n^2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 23, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t _gcd_seq(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* For n < 2 the OEIS offset is 2; the behavior may be undefined,
       but we return 0 for simplicity. */
    if (n < 2)
        return 0;

    /* m = n^2 as seq_t */
    seq_t m = (seq_t)n * (seq_t)n;

    /* Compute C(m,4) multiplicatively with reduction to avoid overflow. */
    seq_t bin = 1;
    for (idx_t i = 1; i <= 4; ++i) {
        /* numerator is m - 4 + i = m - (4 - i) */
        seq_t num   = m - (4 - i);
        seq_t denom = i;

        /* cancel common factor between bin and denom */
        seq_t g1 = _gcd_seq(bin, denom);
        bin /= g1;
        denom /= g1;

        /* cancel remaining factor between num and denom */
        seq_t g2 = _gcd_seq(num, denom);
        num /= g2;
        denom /= g2;   /* denom should now be 1 */

        bin *= num;
    }

    /* a(n) = C(n^2,4) + n^2 */
    return bin + m;
}
