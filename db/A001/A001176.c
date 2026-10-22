/*
 * A001176  Number of zeros in fundamental period of Fibonacci numbers mod n.
 * https://oeis.org/A001176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of zeros in the Pisano period modulo n
 * Range:     u8 n <= 358, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 1;
    
    seq_t mod = (seq_t)n;
    seq_t f_prev = 0, f_curr = 1;
    seq_t zero_count = 0;
    
    do {
        if (f_prev == 0) zero_count++;
        seq_t f_next = (f_prev + f_curr) % mod;
        f_prev = f_curr;
        f_curr = f_next;
    } while (f_prev != 0 || f_curr != 1);
    
    return zero_count;
}
