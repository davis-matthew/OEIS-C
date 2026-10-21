/*
 * A098457  Farey Bisection Expansion of sqrt(7).
 * https://oeis.org/A098457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(2) = 1, a(3) = 0, a(4) = 1, a(5) = 0, a(6) = 1, a(7)
 *            = 1 a(n) = a(n-7) for n > 7 (period‑7 repetition)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* period‑7 pattern */
    static const seq_t tbl[7] = {1, 1, 0, 1, 0, 1, 1};
    idx_t i = (n - 1) % 7;      /* n≥1, so n-1 fits in idx_t */
    return tbl[i];
}
