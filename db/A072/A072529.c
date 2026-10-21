/*
 * A072529  Product of the next n multiples of n.
 * https://oeis.org/A072529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^n * (n*(n+1)/2)! / (n*(n-1)/2)!
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If the function is called with n < 1, there's no defined term.
       Returning 0 avoids undefined behaviour; the offset is 1. */
    if (n == 0) return (seq_t)0;

    seq_t result = (seq_t)1;
    seq_t n_seq = (seq_t)n;
    /* m = n*(n-1)/2   -- the lower bound of the factorial ratio */
    seq_t m = (n_seq * (n_seq - (seq_t)1)) / (seq_t)2;

    /* Compute a(n) = n^n * Π_{i=1}^n (m + i) */
    for (idx_t i = (idx_t)1; i <= n; ++i)
    {
        seq_t term = m + (seq_t)i;
        result *= term;
        result *= n_seq;          /* Multiply by n each iteration to accumulate n^n */
    }

    return result;
}
