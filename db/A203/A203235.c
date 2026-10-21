/*
 * A203235  (n-1)-st elementary symmetric function of the first n terms of the
 *          periodic sequence (1,1,1,3,1,1,1,3,...).
 * https://oeis.org/A203235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n if floor(n/4) = 0 * a(n) = 3^(floor(n/4)-1) * (3*n -
 *            2*floor(n/4)) otherwise * (equivalent to the recurrence a(n) =
 *            6*a(n-4) - 9*a(n-8) )
 * Range:     u8 n <= 11, u16 n <= 31, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute q = floor(n/4) */
    idx_t q = n / 4;

    /* Case with no factor 3 */
    if (q == 0)
        return (seq_t)n;

    /* Compute 3^(q-1) */
    seq_t pow3 = 1;
    for (idx_t i = 0; i < q - 1; ++i)
        pow3 *= (seq_t)3;

    /* Compute the multiplier (3*n - 2*q) */
    seq_t term = (seq_t)3 * (seq_t)n - (seq_t)2 * (seq_t)q;

    /* Result */
    return pow3 * term;
}
