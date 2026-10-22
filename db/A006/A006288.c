/*
 * A006288  Loxton-van der Poorten sequence: base-4 representation contains
 *          only -1, 0, +1.
 * https://oeis.org/A006288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(3n) = 4a(n), a(3n+1) = 4a(n) + 1, a(3n+2) = 4a(n+1) - 1, a(0)
 *            = 0, a(1) = 1
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    
    idx_t q = n / 3;
    idx_t r = n % 3;
    
    seq_t a_q = a(q);
    
    if (r == 0) {
        return 4 * a_q;
    } else if (r == 1) {
        return 4 * a_q + 1;
    } else { // r == 2
        seq_t a_q_plus_1 = a(q + 1);
        return 4 * a_q_plus_1 - 1;
    }
}
