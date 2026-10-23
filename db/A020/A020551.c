/*
 * A020551  5th Fibonacci polynomial evaluated at x = n!.
 * https://oeis.org/A020551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n!)^4 + 3*(n!)^2 + 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n! iteratively. */
    seq_t fact = (seq_t)1;
    for (idx_t i = (idx_t)2; i <= n; ++i) {
        fact *= (seq_t)i;
    }

    /* Compute (n!)^2 and then (n!)^4. */
    seq_t sq  = fact * fact;          /* (n!)^2 */
    seq_t ans = sq * sq + 3 * sq + (seq_t)1;  /* (n!)^4 + 3*(n!)^2 + 1 */

    return ans;
}
