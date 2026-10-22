/*
 * A003608  Add 4, then reverse digits; start with 0.
 * https://oeis.org/A003608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sequence starting at 0, iteratively applying "add 4,
 *            reverse digits"
 * Range:     u8 n <= 14, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t reverse_digits(seq_t x) {
    seq_t result = 0;
    while (x > 0) {
        result = result * 10 + (x % 10);
        x /= 10;
    }
    return result;
}

seq_t a(idx_t n) {
    static seq_t cache[61];
    static int cached_count = 0;
    
    if (cached_count == 0) {
        cache[0] = 0;
        seq_t value = 0;
        for (int i = 1; i < 61; i++) {
            value = value + 4;
            value = reverse_digits(value);
            cache[i] = value;
        }
        cached_count = 61;
    }
    
    if (n >= 61) {
        n = 7 + ((n - 7) % 54);
    }
    
    return cache[n];
}
