/*
 * A226598  a(n) = c(n,n-1,...,1), the Cantor tuple function c applied to
 *          n-tuple (n,n-1,...,1).
 * https://oeis.org/A226598
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = c(n,n-1,...,1) computed via repeated Cantor pairing
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle base cases explicitly to avoid overflows when n==0. */
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    seq_t result = (seq_t)n;        /* current value of c(...). */
    for (idx_t k = n - 1; k > 0; --k)
    {
        seq_t s = result + (seq_t)k;
        result = s * (s + 1) / 2 + (seq_t)k;   /* Cantor pair (result, k). */
    }
    return result;
}
