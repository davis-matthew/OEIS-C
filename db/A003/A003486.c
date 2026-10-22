/*
 * A003486  a(n) = (n^2 + 1)*3^n.
 * https://oeis.org/A003486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 + 1)*3^n
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 15, u64 n <= 33, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t poly = n_seq * n_seq + 1;
    
    // Compute 3^n using binary exponentiation
    seq_t power = 1;
    seq_t base = 3;
    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) {
            power = power * base;
        }
        base = base * base;
        exp >>= 1;
    }
    
    return poly * power;
}
