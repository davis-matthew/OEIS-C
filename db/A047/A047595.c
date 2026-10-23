/*
 * A047595  Numbers that are congruent to {0, 1, 2, 3, 4, 5, 7} mod 8.
 * https://oeis.org/A047595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-1 + floor(n/7)
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A047595: Numbers congruent to {0,1,2,3,4,5,7} mod 8. */
seq_t a(idx_t n)
{
    /* offset is 1: a(1)=0, so n>=1 */
    seq_t result = (seq_t)(n - (idx_t)1);   /* n-1 */
    result += (seq_t)(n / (idx_t)7);        /* floor(n/7) */
    return result;
}
