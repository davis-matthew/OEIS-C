/*
 * A025786  Expansion of 1/((1-x)(1-x^7)(1-x^10)).
 * https://oeis.org/A025786
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0}^{⌊n/10⌋} (⌊(n-10k)/7⌋ + 1)
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This counts the number of non‑negative integer solutions to i+7j+10k = n,
   i is implicit as n-7j-10k >= 0.  The implementation runs in O(n/10)
   time and fits within seq_t for all n up to several thousands. */

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t k = 0; k <= n / 10; ++k)
    {
        idx_t rem = n - 10 * k;        /* remaining after using 10k */
        idx_t jmax = rem / 7;          /* maximum j for this k */
        result += (seq_t)(jmax + 1);   /* count all j from 0..jmax */
    }
    return result;
}
