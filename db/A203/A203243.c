/*
 * A203243  Second elementary symmetric function of the first n terms of
 *          (1,3,9,27,81,...).
 * https://oeis.org/A203243
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3 - 4*3^n + 9^n)/16
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 2 according to the specification.
       For safety, if n < 2 we simply return 0. */
    if (n < 2) return (seq_t)0;

    /* Compute 3^n and 9^n iteratively.  This is O(n) and
       works for all allowed seq_t types, including arbitrary
       precision integers. */
    seq_t pow3 = (seq_t)1;
    seq_t pow9 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        pow3 *= (seq_t)3;
        pow9 *= (seq_t)9;
    }

    /* numerator = 9^n - 4*3^n + 3, safe from underflow because
       9^n > 4*3^n for all n >= 2. */
    seq_t numerator = pow9;
    numerator -= (seq_t)4 * pow3;
    numerator += (seq_t)3;

    /* Final division by 16. */
    return numerator / (seq_t)16;
}
