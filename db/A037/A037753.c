/*
 * A037753  Base 6 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,3,1,0.
 * https://oeis.org/A037753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 6*a(n-1) + d, where d cycles 2,3,1,0 (period 4); a(1) =
 *            2. Off‑set is 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined; safe fallback */

    static const int digits[4] = {2, 3, 1, 0};
    seq_t val = (seq_t)0;
    for (idx_t i = 1; i <= n; ++i) {
        int d = digits[(i - 1) % 4];
        val = val * (seq_t)6 + (seq_t)d;
    }
    return val;
}
