/*
 * A002810  Smallest number containing n syllables in UK English.
 * https://oeis.org/A002810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  word, nonn
 * Formula:   a(n+12) = a(n)*1000+777
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 0) return 0;
    
    // First 12 terms (smallest numbers with 1-12 syllables in UK English)
    static const seq_t first_12[] = {0, 1, 7, 11, 27, 77, 107, 111, 127, 177, 777, 1127, 1177};
    
    if (n <= 12) return first_12[n];
    
    // Recursive formula: a(n) = a(n-12) * 1000 + 777
    // This works because multiplying by 1000 and adding 777 appends
    // "thousand ... seven hundred and seventy seven" which adds 12 syllables:
    // "thousand"(2) + "seven"(2) + "hundred"(2) + "and"(1) + "seventy"(3) + "seven"(2) = 12
    seq_t base = a(n - 12);
    return base * 1000 + 777;
}
