/*
 * A016123  a(n) = (11^(n+1) - 1)/10.
 * https://oeis.org/A016123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = (11^(n+1) - 1) / 10. Recurrence: a(0) = 1; a(n) = 11 *
 *            a(n-1) + 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;           /* holds a(k) for the current k */
    for (idx_t i = 1; i <= n; ++i) {
        res = res * 11 + 1;  /* update to a(i) */
    }
    return res;
}
