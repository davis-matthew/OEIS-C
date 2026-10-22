/*
 * A007907  Concatenation of sequence (1, 2, ..., floor((n-1)/2), floor(n/2),
 *          floor(n/2)-1, ..., 1) for n >= 1.
 * https://oeis.org/A007907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Concatenation of (1, 2, ..., ceil(n/2), floor(n/2), ..., 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static idx_t count_digits(idx_t x) {
    idx_t count = 0;
    if (x == 0) return 1;
    while (x > 0) {
        count++;
        x /= 10;
    }
    return count;
}

static seq_t pow10(idx_t exp) {
    seq_t result = 1;
    for (idx_t i = 0; i < exp; i++) {
        result *= 10;
    }
    return result;
}

seq_t a(idx_t n) {
    idx_t ceil_half = (n + 1) / 2;
    idx_t floor_half = n / 2;
    
    seq_t result = 0;
    
    for (idx_t i = 1; i <= ceil_half; i++) {
        result = result * pow10(count_digits(i)) + (seq_t)i;
    }
    
    for (idx_t i = floor_half; i > 0; i--) {
        result = result * pow10(count_digits(i)) + (seq_t)i;
    }
    
    return result;
}
