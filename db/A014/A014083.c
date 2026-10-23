/*
 * A014083  Occurrences of '1111' in binary expansion of n.
 * https://oeis.org/A014083
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 9144, u16 n <= 9144, u32 n <= 9144, u64 n <= 9144,
 *            u128 n <= 9144, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(2n) = a(n); a(2n+1) = a(n) + [n mod 8 == 7]; a(0) = 0 */
seq_t a(idx_t n)
{
    seq_t count = 0;
    while (n > 0) {
        if (n & 1) { /* n = 2k + 1 */
            count += ((n >> 1) & 7) == 7;
        }
        n >>= 1;
    }
    return count;
}
