/*
 * A009978  Powers of 34.
 * https://oeis.org/A009978
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 34^n Recurrence: a(0) = 1; a(n) = 34 * a(n-1) for n > 0
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = (seq_t)1;  /* a(0) */
    for (idx_t i = 0; i < n; ++i) {
        res = res * (seq_t)34;
    }
    return res;
}
