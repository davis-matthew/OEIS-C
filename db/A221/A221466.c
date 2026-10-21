/*
 * A221466  Number of 0..n arrays of length 7 with each element unequal to at
 *          least one neighbor, starting with 0.
 * https://oeis.org/A221466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^4 * (n + 1) * (n + 3) (OEIS A221466)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 39, u64 n <= 135, u128 n <= 135,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /*  Offset is 1: a(1) = 8. For n < 1 behaviour is unspecified. */
    if (n < 1)   /* n is idx_t unsigned; this check is safe */
        return 0;

    seq_t k = (seq_t)n;            /* cast to seq_t for the computation */
    seq_t res = k * k * k * k;     /* k^4  */
    res *= (k + 1);                /* k^4 * (k+1)  */
    res *= (k + 3);                /* k^4 * (k+1) * (k+3) */
    return res;
}
