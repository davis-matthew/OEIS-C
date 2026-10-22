/*
 * A004489  Table of tersums m + n (answers written in base 10).
 * https://oeis.org/A004489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabl, look, easy, nice
 * Formula:   tersum(col, row-col) where tersum adds base-3 digits mod 3 with
 *            no carries
 * Range:     u8 n <= 9461, u16 n <= 9461, u32 n <= 9461, u64 n <= 9461,
 *            u128 n <= 9461, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t tersum(seq_t x, seq_t y) {
    seq_t result = 0;
    seq_t power = 1;
    while (x > 0 || y > 0) {
        seq_t digit_x = x % 3;
        seq_t digit_y = y % 3;
        seq_t digit_sum = (digit_x + digit_y) % 3;
        result += digit_sum * power;
        power *= 3;
        x /= 3;
        y /= 3;
    }
    return result;
}

seq_t a(idx_t n) {
    idx_t r = 0;
    idx_t count = 1;
    idx_t row_start = 0;
    
    while (row_start + count <= n) {
        row_start += count;
        count++;
        r++;
    }
    
    idx_t pos = n - row_start;
    seq_t m = (seq_t)pos;
    seq_t k = (seq_t)r;
    seq_t other = k - m;
    
    return tersum(m, other);
}
