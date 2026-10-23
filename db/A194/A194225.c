/*
 * A194225  [sum{(k/7) : 1<=k<=n}], where [ ]=floor, ( )=fractional part.
 * https://oeis.org/A194225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( sum_{k=1}^n (k mod 7) / 7 ) = 3 * floor(n/7) +
 *            floor( (n mod 7) * (n mod 7 + 1) / 14 ) (where the division is
 *            integer truncation)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t q = n / 7;          /* floor(n/7)   */
    idx_t r = n % 7;          /* n mod 7      */
    seq_t part1 = (seq_t)q * 3;
    seq_t part2 = (seq_t)r * (r + 1) / 14;  /* integer division truncates, same as floor */
    return part1 + part2;
}
