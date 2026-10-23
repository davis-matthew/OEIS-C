/*
 * A198517  Period 5: repeat [1,0,1,0,0].
 * https://oeis.org/A198517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [1,0,1,0,0][n mod 5]
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n % 5ULL) {
        case 0: return 1;
        case 1: return 0;
        case 2: return 1;
        case 3: return 0;
        case 4: return 0;
    }
    return 0; /* unreachable */
}
