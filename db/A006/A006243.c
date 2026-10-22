/*
 * A006243  Extracting a square root.
 * https://oeis.org/A006243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 198, a(n) = a(n-1)^3 - 3*a(n-1)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 2, u128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t current = (seq_t)198;
    
    for (idx_t i = 2; i <= n; i++) {
        seq_t next = current * current * current - 3 * current;
        current = next;
    }
    
    return current;
}
