/*
 * A007943  Concatenation of sequence (1,3,..,2n-1,2n,2n-2,..,2).
 * https://oeis.org/A007943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   concatenate (1, 3, 5, ..., 2n-1, 2n, 2n-2, ..., 4, 2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static idx_t count_digits(seq_t n) {
    if (n == 0) return 1;
    idx_t count = 0;
    seq_t temp = n;
    while (temp > 0) {
        count++;
        temp /= 10;
    }
    return count;
}

seq_t a(idx_t n) {
    seq_t result = 0;
    
    // Concatenate odd numbers: 1, 3, 5, ..., 2n-1
    for (idx_t i = 1; i <= n; i++) {
        seq_t num = 2 * i - 1;
        idx_t digits = count_digits(num);
        seq_t multiplier = 1;
        for (idx_t j = 0; j < digits; j++) {
            multiplier *= 10;
        }
        result = result * multiplier + num;
    }
    
    // Concatenate even numbers in reverse: 2n, 2n-2, ..., 4, 2
    for (idx_t i = n; i > 0; i--) {
        seq_t num = 2 * i;
        idx_t digits = count_digits(num);
        seq_t multiplier = 1;
        for (idx_t j = 0; j < digits; j++) {
            multiplier *= 10;
        }
        result = result * multiplier + num;
    }
    
    return result;
}
