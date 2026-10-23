/*
 * A125203  Number of ways to write n as 4*x*y - x - y with 1<=x<=y.
 * https://oeis.org/A125203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pairs (x, y) with 1 ≤ x ≤ y such that 4*x*y - x
 *            - y == n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* defensive: OEIS offset is 1 */
    seq_t count = 0;
    idx_t y_max = (n + 1) / 3;        /* minimal value for given y is 3*y-1 */
    for (idx_t y = 1; y <= y_max; ++y) {
        idx_t denom = 4 * y - 1;      /* 4y - 1 */
        idx_t num   = n + y;          /* n + y */
        if (num % denom != 0) continue; /* not an integer x */
        idx_t x = num / denom;
        if (x <= y) /* x >= 1 automatically holds for y ≥ 1 */
            ++count;
    }
    return count;
}
