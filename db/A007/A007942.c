/*
 * A007942  Decimal concatenation of sequence (n, n-1, ..., 2, 1, 2, ..., n-1,
 *          n).
 * https://oeis.org/A007942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   Decimal concatenation of sequence (n, n-1, ..., 1, 2, ..., n-1,
 *            n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Validated: standard, pass, 30 terms through n = 198 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t concat_number(seq_t current, idx_t num) {
    idx_t temp = num;
    int digits = 0;
    if (temp == 0) digits = 1;
    else {
        while (temp > 0) {
            digits++;
            temp /= 10;
        }
    }
    
    idx_t divisor = 1;
    for (int d = 1; d < digits; d++) {
        divisor *= 10;
    }
    
    for (int d = 0; d < digits; d++) {
        seq_t digit = (seq_t)((num / divisor) % 10);
        current = current * 10 + digit;
        if (divisor > 1) divisor /= 10;
    }
    
    return current;
}

seq_t a(idx_t n) {
    seq_t result = 0;
    
    for (idx_t i = n; i > 0; i--) {
        result = concat_number(result, i);
    }
    
    for (idx_t i = 2; i <= n; i++) {
        result = concat_number(result, i);
    }
    
    return result;
}
