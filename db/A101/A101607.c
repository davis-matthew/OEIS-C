/*
 * A101607  a(2n) = 7 - a(n), a(2n+1) = (n-1 mod 3) + 1.
 * https://oeis.org/A101607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
 *   a(2n)   = 7 - a(n)
 *   a(2n+1) = (n % 3) + 1   (where n = (i-1)/2 for odd i)
 *   a(1) = 1
 */
seq_t a(idx_t n)
{
    if ((n & 1) == 0)          /* even index */
    {
        seq_t sub = a(n / 2);
        return (seq_t)7 - sub;
    }
    else                      /* odd index */
    {
        seq_t k = (seq_t)((n - 1) / 2);
        return (k % 3) + 1;
    }
}
