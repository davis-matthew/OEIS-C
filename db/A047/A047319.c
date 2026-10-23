/*
 * A047319  Numbers that are congruent to {5, 6} mod 7.
 * https://oeis.org/A047319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7 * floor((n-1)/2) + (n%2 ? 5 : 6)
 * Range:     u8 n <= 72, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* pair index: floor((n-1)/2) */
    idx_t pair_idx = (n - 1u) >> 1;          /* n is at least 1 */
    seq_t pair = (seq_t)pair_idx;

    /* base value */
    seq_t res = pair * (seq_t)7;

    /* add 5 or 6 depending on parity */
    if (n & 1u)
        res += (seq_t)5;
    else
        res += (seq_t)6;

    return res;
}
