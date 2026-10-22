/*
 * A004493  Tersum n + 4.
 * https://oeis.org/A004493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = tersum(n + 4), where tersum adds in base 3 mod 3 with no
 *            carries
 * Range:     u8 n <= 251, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t base3_n[64];
    int len_n = 0;
    seq_t temp = (seq_t)n;
    
    if (temp == 0) {
        base3_n[0] = 0;
        len_n = 1;
    } else {
        while (temp > 0) {
            base3_n[len_n] = temp % 3;
            len_n++;
            temp /= 3;
        }
    }
    
    // 4 in base 3 is [1, 1]
    seq_t base3_4[2] = {(seq_t)1, (seq_t)1};
    int len_4 = 2;
    
    // Tersum: add mod 3
    seq_t result[64];
    int max_len = (len_n > len_4) ? len_n : len_4;
    
    for (int i = 0; i < max_len; i++) {
        seq_t digit_n = (i < len_n) ? base3_n[i] : (seq_t)0;
        seq_t digit_4 = (i < len_4) ? base3_4[i] : (seq_t)0;
        result[i] = (digit_n + digit_4) % 3;
    }
    
    // Convert from base 3 to decimal
    seq_t output = (seq_t)0;
    seq_t power = (seq_t)1;
    for (int i = 0; i < max_len; i++) {
        output += result[i] * power;
        power *= 3;
    }
    
    return output;
}
