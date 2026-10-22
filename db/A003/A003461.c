/*
 * A003461  Bode numbers multiplied by 10: 4 + 3*floor(2^(n-1)).
 * https://oeis.org/A003461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 4 + 3*floor(2^(n-1))
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)4;
    
    seq_t power_of_2 = (seq_t)1;
    for (idx_t i = 1; i < n; i++) {
        power_of_2 *= 2;
    }
    
    return (seq_t)4 + 3 * power_of_2;
}
