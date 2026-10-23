/*
 * A021522  Decimal expansion of 1/518.
 * https://oeis.org/A021522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = 0 for n = 0 or 1; a(n) = {1,9,3,0,5,0}[ (n-2) mod 6 ] for
 *            n ≥ 2
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) {
        return 0;
    }
    static const seq_t tbl[6] = {1, 9, 3, 0, 5, 0};
    return tbl[(n - 2) % 6];
}
