/*
 * A067471  n-th root of A067470(n).
 * https://oeis.org/A067471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, fini, full, nonn
 * Formula:   a(n) = the smallest integer m such that m^n has at least n
 *            decimal digits, equivalently a(n) = ceil(10^((n-1)/n)). This
 *            yields the following thresholds for m in [1..10]: n = 1 -> 1 n =
 *            2 -> 4 n = 3 -> 5 n = 4 -> 6 n = 5..6 -> 7 n = 7..10 -> 8 n =
 *            11..21 -> 9 n >= 22 -> 10
 * Range:     u8 n <= 21, u16 n <= 21, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)              return 1;
    if (n <= 2)              return 4;
    if (n <= 3)              return 5;
    if (n <= 4)              return 6;
    if (n <= 6)              return 7;
    if (n <= 10)             return 8;
    if (n <= 21)             return 9;
    return 10;               /* n >= 22 */
}
