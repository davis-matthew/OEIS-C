/*
 * A047602  Numbers that are congruent to {0, 1, 2, 3, 4, 5} mod 8.
 * https://oeis.org/A047602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((8/7)*floor(7*(n-1)/6))
 * Range:     u8 n <= 192, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1‑based.  For n = 0 the sequence is undefined, return 0 for safety. */
    if (n == 0u) return 0;

    /* Use idx_t for the intermediate arithmetic to keep values within 64‑bit
       range when n is reasonable, and cast to seq_t only for the final step.   */
    idx_t tmp = n - 1;
    idx_t t1  = tmp * (idx_t)7;            /* 7*(n-1)          */
    idx_t t2  = t1 / (idx_t)6;            /* floor(7*(n-1)/6) */

    /* Convert to seq_t and perform the final division.         */
    seq_t  res = (seq_t)8 * (seq_t)t2 / (seq_t)7;
    return res;
}
