/*
 * A007601  Positions where A007600 increases.
 * https://oeis.org/A007601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(n) = A000792(n-1) + 1 for n > 1
 * Range:     u8 n <= 16, u16 n <= 31, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    
    idx_t m = n - 1;
    
    if (m == 1) return (seq_t)2;
    if (m == 2) return (seq_t)3;
    if (m == 3) return (seq_t)4;
    if (m == 4) return (seq_t)5;
    
    idx_t k = (m - 5) / 3 + 1;
    idx_t pos = (m - 5) % 3;
    
    seq_t pow3 = (seq_t)1;
    for (idx_t i = 0; i < k; i++) {
        pow3 = pow3 * (seq_t)3;
    }
    
    seq_t a000792_val;
    if (pos == 0) {
        a000792_val = (seq_t)2 * pow3;
    } else if (pos == 1) {
        a000792_val = (seq_t)3 * pow3;
    } else {
        a000792_val = (seq_t)4 * pow3;
    }
    
    return a000792_val + (seq_t)1;
}
