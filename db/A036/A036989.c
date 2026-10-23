/*
 * A036989  Read binary expansion of n from the right; keep track of the
 *          excess of 1's over 0's that have been seen so far; sequence gives
 *          1 + maximum(excess of 1's over 0's).
 * https://oeis.org/A036989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(2n) = max(a(n) - 1, 1); a(2n+1) = a(n) + 1;
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    if (n & 1) {                 /* odd: n = 2k+1 */
        return a(n >> 1) + (seq_t)1;
    } else {                     /* even: n = 2k */
        seq_t v = a(n >> 1);
        return v > (seq_t)1 ? v - (seq_t)1 : (seq_t)1;
    }
}
