/*
 * A061882  a(n) = Smallest nontrivial number k > 9 such that first (leftmost)
 *          digit - second digit + third digit - fourth digit ... of k = n.
 * https://oeis.org/A061882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = smallest integer k > 9 with alternating digit sum equal n
 * Range:     u8 n <= 11, u16 n <= 24, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation constructs minimal-length number by using a leading digit followed by
   repeating pairs of 0 and 9 to reach the desired sum. */
seq_t a(idx_t n)
{
    /* n == 0 is a special case: the smallest k > 9 with sum 0 is 11. */
    if (n == 0)
        return (seq_t)11;

    /* For 1 ≤ n ≤ 9 we can use two digits: d0 and 0. */
    if (n < 10) {
        seq_t x = (seq_t)n;          /* leading digit d0 = n */
        return x * 10;               /* trailing zero guarantees k > 9   */
    }

    /* For larger n the minimal length is odd.  Let kPairs be the number of
       0‑9 pairs after the leading digit.  Then the alternating sum is
       d0 + 9*kPairs = n.  We want the smallest kPairs such that
       n ≤ 9*(kPairs+1), i.e. ceil(n/9)-1.                                         */
    idx_t kPairs = ((n + 8) / 9) - 1;   /* ceil(n/9) - 1 */
    idx_t len    = 2 * kPairs + 1;      /* total number of digits (odd) */
    idx_t d0     = n - 9 * kPairs;     /* leading digit (1..9)             */

    seq_t result = (seq_t)d0;
    for (idx_t i = 1; i < len; ++i) {
        seq_t digit = (i & 1) ? 0 : 9;   /* odd indices -> 0, even -> 9 */
        result = result * 10 + digit;
    }
    return result;
}
