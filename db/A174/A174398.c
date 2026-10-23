/*
 * A174398  Numbers that are congruent to {1, 4, 5, 8} mod 12.
 * https://oeis.org/A174398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n - 3 + (-1)^floor((n-1)/2)
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent succinct form: a(n) = 3*n - 2  (k even) , a(n) = 3*n - 4  (k odd),
   where k = floor((n-1)/2) */

seq_t a(idx_t n)
{
    /* k is floor((n-1)/2) */
    idx_t k = (n - 1) >> 1;
    /* compute 3*n as seq_t to avoid overflow on n */
    seq_t three_n = 3 * (seq_t)n;
    /* subtract 2 if k even, otherwise subtract 4 */
    return three_n - (seq_t)((k & 1) ? 4 : 2);
}
