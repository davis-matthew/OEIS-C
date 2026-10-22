/*
 * A006968  Number of letters in Roman numeral representation of n.
 * https://oeis.org/A006968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice, easy
 * Formula:   a(n) = length of Roman numeral representation of n
 * Range:     u8 n <= 3999, u16 n <= 3999, u32 n <= 3999, u64 n <= 3999,
 *            u128 n <= 3999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t len = 0;
    
    // Roman numeral values and their letter counts
    const idx_t values[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
    const seq_t letters[] = {1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1};
    
    idx_t num = n;
    
    for (int i = 0; i < 13; i++) {
        while (num >= values[i]) {
            len += letters[i];
            num -= values[i];
        }
    }
    
    return len;
}
