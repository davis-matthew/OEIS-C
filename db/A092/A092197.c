/*
 * A092197  Brevity advantage of "new style" over "old style" Roman numerals.
 * https://oeis.org/A092197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 2·(number of decimal digits equal to 4) + 3·(number of
 *            decimal digits equal to 9)
 * Range:     u8 n <= 3999, u16 n <= 3999, u32 n <= 3999, u64 n <= 3999,
 *            u128 n <= 3999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    idx_t x   = n;
    while (x > 0) {
        unsigned d = x % 10;
        if (d == 4) res += 2;          /* old uses 4 letters, new uses 2 */
        else if (d == 9) res += 3;     /* old uses 5 letters, new uses 2 */
        x /= 10;
    }
    return res;
}
