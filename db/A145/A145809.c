/*
 * A145809  Number of islands of ones fitting in an n X n array with all ones
 *          connected only either three adjacent vertically or three adjacent
 *          horizontally.
 * https://oeis.org/A145809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n^2/6) with corrections: +1 if n % 6 == 3, -1 if n
 *            % 6 == 4 and n >= 10
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sq = (seq_t)n * (seq_t)n;
    seq_t res = sq / 6;
    idx_t r = n % 6;
    if (r == 3) {
        res += 1;
    } else if (r == 4 && n >= 10) {
        res -= 1;
    }
    return res;
}
