/*
 * A001743  Numbers in which every digit contains at least one loop (version
 *          1).
 * https://oeis.org/A001743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   n-1 in base 4, map digits 0->0, 1->6, 2->8, 3->9, interpret as
 *            base 10
 * Range:     u8 n <= 16, u16 n <= 320, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n - 1;
    seq_t result = 0;
    seq_t power = 1;
    
    static const seq_t digits[] = {0, 6, 8, 9};
    
    while (m > 0) {
        idx_t digit = m % 4;
        result += digits[digit] * power;
        power *= 10;
        m /= 4;
    }
    
    return result;
}
