/*
 * A010796  a(n) = n!*(n+1)!/2.
 * https://oeis.org/A010796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * (n+1)! / 2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is unsigned (idx_t). For the OEIS offset, a(1) = 1. */
    if (n == 0)
    {
        /* For n < 1 the sequence is undefined; return 0 as a safe default. */
        return 0;
    }

    seq_t result = 1;
    /* Use the product formula:
       a(n) = ∏_{k=1}^{n-1} (k^2 + 3k + 2)
     */
    for (idx_t k = 1; k < n; ++k)
    {
        seq_t k_seq = (seq_t)k;
        seq_t term = k_seq * k_seq + (seq_t)3 * k_seq + (seq_t)2;
        result *= term;
    }
    return result;
}
