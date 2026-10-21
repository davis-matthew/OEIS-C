/*
 * A087231  a(n) is the smallest number such that the exponent of p=2 factor
 *          in 6*a(n)+4 equals n.
 * https://oeis.org/A087231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = a(n-1) + 4*a(n-2) - 4*a(n-3)
 * with a(1)=1, a(2)=4, a(3)=6, a(4)=2, a(5)=26
 */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)6;
    if (n == 4) return (seq_t)2;
    if (n == 5) return (seq_t)26;

    seq_t prev3 = (seq_t)6;   /* a(3)   */
    seq_t prev2 = (seq_t)2;   /* a(4)   */
    seq_t prev1 = (seq_t)26;  /* a(5)   */
    seq_t curr = 0;

    for (idx_t i = 6; i <= n; ++i)
    {
        curr = prev1 + (prev2 << 2) - (prev3 << 2);
        prev3 = prev2;
        prev2 = prev1;
        prev1 = curr;
    }

    return curr;
}
