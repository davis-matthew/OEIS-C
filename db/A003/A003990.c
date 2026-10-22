/*
 * A003990  Table of lcm(x,y), read along antidiagonals.
 * https://oeis.org/A003990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, nonn, easy, nice, look
 * Formula:   a(n) = lcm(x,y), where x,y come from reading antidiagonals of
 *            lcm table
 * Range:     u8 n <= 271, u16 n <= 4950, u32 n <= 4950, u64 n <= 4950,
 *            u128 n <= 4950, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t gcd(seq_t a, seq_t b) {
    while (b != 0) {
        seq_t temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

static seq_t lcm(seq_t a, seq_t b) {
    return a / gcd(a, b) * b;
}

seq_t a(idx_t n) {
    seq_t k = (seq_t)1;
    while (k * (k + 1) / 2 < (seq_t)n) {
        k++;
    }
    
    seq_t pos = (seq_t)n - k * (k - 1) / 2;
    seq_t x = pos;
    seq_t y = k + 1 - pos;
    
    return lcm(x, y);
}
