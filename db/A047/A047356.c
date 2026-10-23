/*
 * A047356  Numbers that are congruent to {1, 3} mod 7.
 * https://oeis.org/A047356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7 * ((n-1)/2) + (n%2?1:3)
 * Range:     u8 n <= 74, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)            /* behaviour below the offset is undefined */
        return 0;
    idx_t m = (n - 1) / 2; /* floor((n-1)/2) */
    seq_t val = 7 * ((seq_t)m);
    if (n & 1)             /* odd index: add 1 */
        val += 1;
    else                   /* even index: add 3 */
        val += 3;
    return val;
}
