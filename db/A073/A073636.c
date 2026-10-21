/*
 * A073636  Period 3: repeat [1, 8, 9] ; Digital root of A000578(n) = n^3 for
 *          n >= 1.
 * https://oeis.org/A073636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = {1, 8, 9}[ (n-1) mod 3 ] (Period 3)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t tbl[3] = { (seq_t)1, (seq_t)8, (seq_t)9 };
    return tbl[(n - 1) % 3];
}
