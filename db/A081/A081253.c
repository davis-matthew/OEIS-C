/*
 * A081253  Numbers k such that A081252(m)/m^2 has a local minimum for m = k.
 * https://oeis.org/A081253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(2^(n-1)*7/3) for n ≥ 1. This is equivalent to the
 *            recurrence: a(1) = 2; a(n) = a(n-1) * 2 + (n % 2) (add 1 when n
 *            is odd, otherwise 0)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* not used, but harmless */
    seq_t val = 2;             /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        val = val * 2;             /* multiply by 2 */
        if (i & 1) val += 1;       /* add 1 for odd indices */
    }
    return val;
}
