/*
 * A082274  Palindromes k such that k + 2 is also a palindrome.
 * https://oeis.org/A082274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 11, u32 n <= 16, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n              for n <= 7
   a(n) = 10^(n-7) - 1   for n >  7
   The computation below builds the number of n‑7 trailing 9’s
   by repeated multiplication by 10 and addition of 9. */
seq_t a(idx_t n)
{
    if (n <= (idx_t)7)
        return (seq_t)n;

    idx_t k = n - (idx_t)7;          /* number of 9’s in the result */
    seq_t val = (seq_t)0;            /* start at 0 */

    for (idx_t i = (idx_t)0; i < k; ++i)
        val = val * (seq_t)10 + (seq_t)9;

    return val;
}
