/*
 * A001633  Numbers with an odd number of digits.
 * https://oeis.org/A001633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   The n-th number with an odd number of digits
 * Range:     u8 n <= 166, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 72 terms through n = 977 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static idx_t count_for_digits(int digits) {
    if (digits == 1) return 10;
    idx_t result = 9;
    for (int i = 1; i < digits; i++) {
        result *= 10;
    }
    return result;
}

static seq_t pow10(int k) {
    seq_t result = 1;
    for (int i = 0; i < k; i++) {
        result *= 10;
    }
    return result;
}

seq_t a(idx_t n) {
    idx_t pos = n;
    int digits = 1;
    idx_t group_size = 10;
    
    // Find which digit-group contains the n-th term
    while (pos > group_size) {
        pos -= group_size;
        digits += 2;
        group_size = count_for_digits(digits);
    }
    
    // Compute the first number with 'digits' digits
    seq_t start = (digits == 1) ? 0 : pow10(digits - 1);
    
    // Return the pos-th number in this group (1-indexed within group)
    return start + (seq_t)(pos - 1);
}
