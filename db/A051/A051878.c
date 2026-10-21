/*
 * A051878  Partial sums of A051797.
 * https://oeis.org/A051878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+4,4)*(8*n+5)/5
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 143, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implemented by cancelling the denominator 120 = 2^3*3*5 from the product
 * (n+1)(n+2)(n+3)(n+4)(8n+5) to avoid overflow on any integer `seq_t`. */

seq_t a(idx_t n)
{
    /* create numerator factors */
    seq_t f[5];
    f[0] = (seq_t)(n + 1);
    f[1] = (seq_t)(n + 2);
    f[2] = (seq_t)(n + 3);
    f[3] = (seq_t)(n + 4);
    /* the 8*n term must be computed in seq_t to avoid overflow of idx_t */
    f[4] = ((seq_t)8 * (seq_t)n) + (seq_t)5;

    /* cancel denominator factors 2^3, 3, 5 */
    const unsigned int den[5] = {2, 2, 2, 3, 5};
    for (unsigned int d = 0; d < 5; ++d) {
        for (int i = 0; i < 5; ++i) {
            if (f[i] % den[d] == 0) {
                f[i] /= den[d];
                break;
            }
        }
    }

    /* multiply remaining numerator factors */
    seq_t res = 1;
    for (int i = 0; i < 5; ++i)
        res *= f[i];

    return res;
}
