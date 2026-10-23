/*
 * A152653  a(n) = (n-1)! * Product_{k=1..n-1} k!.
 * https://oeis.org/A152653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)! * ∏_{k=1..n-1} k!
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the defined offset: a(1) = 1 */
    if (n == 0) return 0;          /* unused, but safe for n < 1 */

    seq_t res  = 1;                /* product of k! for k = 1..n-1 */
    seq_t fact = 1;                /* factorial accumulator */

    for (idx_t k = 1; k < n; ++k) {
        fact *= (seq_t)k;          /* fact = k! */
        res  *= fact;              /* res *= k! */
    }
    /* multiply by (n-1)! once more */
    res *= fact;

    return res;
}
