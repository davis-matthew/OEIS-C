/*
 * A076160  Sod_4 - sod_3 + sod_2 - sod_1, where sod_k is the sum of k-th
 *          powers of digits of n.
 * https://oeis.org/A076160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{digits d of n} d*(d-1)*(d^2+1) Equivalent to a(n) =
 *            S4(n) - S3(n) + S2(n) - S1(n)
 * Range:     u8 n <= 4, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* Handle n = 0 specially to avoid skipping the first digit */
    if (n == 0)
        return 0;

    while (n > 0)
    {
        unsigned d = n % 10;   /* least significant decimal digit (0..9) */
        n /= 10;

        /* Compute d*(d-1)*(d^2+1) exactly in seq_t */
        seq_t term = (seq_t)d * (d - 1) * ((seq_t)d * d + 1);
        sum += term;
    }
    return sum;
}
