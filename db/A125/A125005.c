/*
 * A125005  Numbers of orders of nontrivial positive magic-squares with magic
 *          sum n. A nontrivial positive magic square is a k-by-k array of
 *          consecutive positive integers (not necessarily including 1) such
 *          that all rows, all columns and the two diagonals each add up to
 *          the same constant (the "magic sum"), wi...
 * https://oeis.org/A125005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of orders k>2 for which a k×k magic square of consecutive
 * positive integers has magic sum n.  For each k we need
 *   k*(k^2+1)/2 ≤ n            (a ≥ 1)
 * and
 *   n % k == 0  if k is odd
 *   n % k == k/2 if k is even
 * The function iterates over k until the lower bound exceed n. */
seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t k = 3; ; ++k)
    {
        seq_t kmk = (seq_t)k * k;
        seq_t n_min = (seq_t)k * (kmk + 1) / 2;   /* k*(k^2+1)/2 */
        if (n_min > (seq_t)n)
            break;
        if ((k & 1U) == 0)
        {
            if (n % k == k / 2)
                ++count;
        }
        else
        {
            if (n % k == 0)
                ++count;
        }
    }
    return count;
}
