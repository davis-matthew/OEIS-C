/*
 * A054127  a(1) = 2; a(n) = 9*2^(n-2) - n - 2, n>1.
 * https://oeis.org/A054127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 2; a(n) = 9*2^(n-2) - n - 2 for n>1
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)2;

    /* Compute 2^(n-2) by repeated doubling.  This avoids UB that would
       arise from shifting by more than the width of seq_t, and works
       uniformly for arbitrary‑precision seq_t. */
    idx_t k = n - 2;
    seq_t pow2 = 1;
    for (idx_t i = 0; i < k; ++i)
        pow2 += pow2;          /* multiply by 2 */

    seq_t res = 9 * pow2;
    res -= n;
    res -= 2;
    return res;
}
