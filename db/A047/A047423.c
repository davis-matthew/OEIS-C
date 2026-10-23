/*
 * A047423  Numbers that are congruent to {2, 3, 4, 5, 6} mod 8.
 * https://oeis.org/A047423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/5) + ((n-1)%5) + 2 (offset 1)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          /* offset guard – behaviour below 1 is not needed */
        return 0;

    idx_t k = (n - 1) / 5;   /* floor((n-1)/5)   */
    idx_t r = (n - 1) % 5;   /* (n-1) mod 5      */

    seq_t res = (seq_t)k * 8; /* 8 * floor((n-1)/5) */
    res += (seq_t)r;          /* add the remainder   */
    res += 2;                 /* final constant offset */
    return res;
}
