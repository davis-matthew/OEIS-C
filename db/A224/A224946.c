/*
 * A224946  Leap years having 53 Mondays and Tuesdays.
 * https://oeis.org/A224946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = base[(n-1)%13] + 400*((n-1)/13) * where base = {1596,
 *            1624, 1652, 1680, 1720, * 1748, 1776, 1816, 1844, 1872, * 1912,
 *            1940, 1968} * Sequence repeats every 13 terms (400‑year cycle)
 * Range:     u8 none, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset 0 */
    static const seq_t base[13] = {
        1596, 1624, 1652, 1680, 1720,
        1748, 1776, 1816, 1844, 1872,
        1912, 1940, 1968
    };
    idx_t pos    = (n - 1) % 13;
    idx_t cycles = (n - 1) / 13;
    return base[pos] + (seq_t)400 * cycles;
}
