/*
 * A047287  Numbers that are congruent to {0, 1, 2, 3, 6} mod 7.
 * https://oeis.org/A047287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n - 5 - 2*(n mod 5))/5 (offset 1)
 * Range:     u8 n <= 184, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For indices below the offset the value is irrelevant.
       The sequence is defined for n >= 1. */
    if (n == 0) return (seq_t)0;   /* safe guard – not used by tests */

    idx_t r    = n % 5;
    idx_t expr = 7 * n - 5 - 2 * r;   /* exact divisible by 5 */
    return (seq_t)(expr / 5);
}
