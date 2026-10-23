/*
 * A120294  Numerator of determinant of n X n matrix with elements M[j,j] =
 *          (i+j)/(i+j-1).
 * https://oeis.org/A120294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = largest prime factor of n^2 + 1 if that factor >= 2n,
 *            otherwise 1
 * Range:     u8 n <= 15, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)                 /* outside the offset, safe default */
        return (seq_t)0;

    /* value = n^2 + 1 */
    seq_t value = (seq_t)n * (seq_t)n + (seq_t)1;

    seq_t largest = (seq_t)1;

    /* Trial division up to sqrt(value) */
    for (idx_t d = 2; (seq_t)d * (seq_t)d <= value; ++d)
    {
        if (value % (seq_t)d == (seq_t)0)
        {
            if ((seq_t)d > largest)
                largest = (seq_t)d;
            while (value % (seq_t)d == (seq_t)0)
                value /= (seq_t)d;
        }
    }

    /* Remaining value is a prime factor > sqrt(original) */
    if (value > (seq_t)1 && value > largest)
        largest = value;

    seq_t two_n = (seq_t)2 * (seq_t)n;
    return (largest >= two_n) ? largest : (seq_t)1;
}
