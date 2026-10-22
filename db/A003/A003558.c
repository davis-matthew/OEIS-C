/*
 * A003558  Least number m > 0 such that 2^m == +-1 (mod 2n + 1).
 * https://oeis.org/A003558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{k > 0 | q_k = 1} where q_0 = 1, q_k = |2n+1 -
 *            2*q_{k-1}|
 * Range:     u8 n <= 64, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t mod_val = 2 * (seq_t)n + 1;
    seq_t q = 1;
    
    for (seq_t k = 1; ; ++k) {
        seq_t doubled = 2 * q;
        seq_t q_new = (doubled > mod_val) ? (doubled - mod_val) : (mod_val - doubled);
        
        if (q_new == 1) {
            return k;
        }
        q = q_new;
    }
}
