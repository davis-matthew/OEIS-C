/*
 * A003137  Write n in base 3 and juxtapose.
 * https://oeis.org/A003137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons, easy, tabf
 * Formula:   a(n) = n-th digit in base-3 Champernowne constant (1, 2, 3, ...
 *            written in base 3 and concatenated)
 * Range:     u8 n <= 5914, u16 n <= 5914, u32 n <= 5914, u64 n <= 5914,
 *            u128 n <= 5914, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t cumulative = 0;
    idx_t digits = 1;
    idx_t count = 2;
    
    while (cumulative + digits * count < n) {
        cumulative += digits * count;
        digits++;
        count *= 3;
    }
    
    idx_t pos_in_segment = n - cumulative;
    idx_t num_index = (pos_in_segment - 1) / digits;
    idx_t digit_index = (pos_in_segment - 1) % digits;
    
    idx_t start = 1;
    for (idx_t i = 1; i < digits; i++) {
        start *= 3;
    }
    idx_t number = start + num_index;
    
    seq_t base3[64];
    int num_digits = 0;
    idx_t temp = number;
    while (temp > 0) {
        base3[num_digits++] = (seq_t)(temp % 3);
        temp /= 3;
    }
    
    return base3[num_digits - 1 - digit_index];
}
