/*
 * A005052  a(n) = 10*3^n.
 * https://oeis.org/A005052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*3^n
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 18, u64 n <= 38, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t base = (seq_t)3;
    seq_t result = (seq_t)1;
    idx_t exp = n;
    
    while (exp > 0) {
        if (exp & 1) {
            result = result * base;
        }
        base = base * base;
        exp = exp >> 1;
    }
    
    return (seq_t)10 * result;
}
