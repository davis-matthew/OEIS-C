/*
 * A004177  Omit 2's from n.
 * https://oeis.org/A004177
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   Remove all '2' digits from the decimal representation of n
 * Range:     u8 n <= 299, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t digits[64];
    int count = 0;
    idx_t temp = n;
    
    while (temp > 0) {
        unsigned int d = (unsigned int)(temp % 10);
        if (d != 2) {
            digits[count++] = (seq_t)d;
        }
        temp /= 10;
    }
    
    seq_t result = 0;
    seq_t power = 1;
    for (int i = 0; i < count; i++) {
        result += digits[i] * power;
        power *= 10;
    }
    
    return result;
}
