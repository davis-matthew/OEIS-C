/*
 * A004468  a(n) = Nim product 3 * n.
 * https://oeis.org/A004468
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = Nim product 3 * n, computed by converting n to base 4,
 *            replacing digits 1→3, 2→1, 3→2, then converting back
 * Range:     u8 n <= 255, u16 n <= 8669, u32 n <= 8669, u64 n <= 8669,
 *            u128 n <= 8669, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t xform[4] = {0, 3, 1, 2};
    seq_t result = 0;
    seq_t power = 1;
    idx_t temp = n;
    
    while (temp > 0) {
        seq_t new_digit = xform[temp % 4];
        result += new_digit * power;
        power *= 4;
        temp /= 4;
    }
    
    return result;
}
