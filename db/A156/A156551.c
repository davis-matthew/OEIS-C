/*
 * A156551  Period 10: repeat [8, 6, 0, 4, 2, 2, 4, 0, 6, 8].
 * https://oeis.org/A156551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = (15*(4 - m^4 + 8*m^3 - 19*m^2 + 12*m) - 2*(m^3 - 6*m^2 +
 *            17*m - 18)*(-1)^floor(n/5))/12 where m = n-5*floor(n/5)
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    uint64_t m = n % 5;    /* m = n mod 5 */
    uint64_t k = n / 5;    /* floor(n/5) */
    int sign = (k & 1) ? -1 : 1;

    uint64_t m2 = m * m;
    uint64_t m3 = m2 * m;
    uint64_t m4 = m3 * m;

    long long part1 = 4 - (long long)m4 + 8 * (long long)m3
                      - 19 * (long long)m2 + 12 * (long long)m;
    long long part2 = (long long)m3 - 6 * (long long)m2 + 17 * (long long)m - 18;

    long long num = 15 * part1 - 2 * part2 * sign;
    return (seq_t)(num / 12);
}
