/*
 * A059482  a(0)=1, a(n) = a(n-1) + 8*10^(n-1).
 * https://oeis.org/A059482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*10^n + 1)/9 Recurrence: a(0) = 1, a(n) = 10*a(n-1) - 1
 *            for n > 0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = (seq_t)1;  /* a(0) */
    for (idx_t i = 0; i < n; ++i) {
        val = val * (seq_t)10 - (seq_t)1;
    }
    return val;
}
