/*
 * A224947  Leap years having 53 Tuesdays and Wednesdays.
 * https://oeis.org/A224947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(n) = base[(n-1) mod 14] + 400 * floor((n-1)/14), * where
 *            base =
 *            {1608,1636,1664,1692,1704,1732,1760,1788,1828,1856,1884,1924,1952,1980}
 * Range:     u8 none, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t base[14] = {
        1608,1636,1664,1692,1704,1732,1760,1788,1828,1856,1884,1924,1952,1980
    };
    if (n == 0)
        return 0;  /* undefined offset, return 0 */
    idx_t k = n - 1;
    idx_t idx = k % 14;
    idx_t cycle = k / 14;
    seq_t val = base[idx] + (seq_t)(400ull * (unsigned long long)cycle);
    return val;
}
