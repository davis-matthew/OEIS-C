/*
 * A061778  a(n) = Product_{j=0..floor(n/2)} binomial(n,j).
 * https://oeis.org/A061778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Product_{j=0..⌊n/2⌋} binomial(n,j)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the off‑by‑one offset: sequence terms start at n = 1 */
    if (n < 1)
        return (seq_t)1;           /* value for n = 0 is unused by the tests */

    /* floor(n/2) gives the highest j to iterates over */
    idx_t half = n >> 1;           /* same as n / 2 for unsigned integers */

    seq_t prod = (seq_t)1;         /* running product of the binomials   */
    seq_t bin  = (seq_t)1;         /* current binomial value             */

    for (idx_t j = 1; j <= half; ++j) {
        /* bin = bin * (n - j + 1) / j   (integer division is exact) */
        seq_t numer = (seq_t)(n - j + 1);
        seq_t denom = (seq_t)j;
        bin = bin * numer / denom;   /* exact division – binomial stays integer */

        prod *= bin;                /* multiply into the total product */
    }
    return prod;
}
