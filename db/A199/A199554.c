/*
 * A199554  a(n) = 6*8^n+1.
 * https://oeis.org/A199554
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=7, a(n)=8*a(n-1)-7 for n>0 */
seq_t a(idx_t n)
{
    /* Handle the base case */
    if (n == 0)
        return (seq_t)7;

    seq_t result = (seq_t)7;
    for (idx_t i = 1; i <= n; ++i) {
        result = result * (seq_t)8 - (seq_t)7;
    }
    return result;
}
