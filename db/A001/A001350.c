/*
 * A001350  Associated Mersenne numbers.
 * https://oeis.org/A001350
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = Lucas(n) - 1 - (-1)^n, where Lucas(n) is A000032(n)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    
    seq_t lucas_prev = (seq_t)2;  // Lucas(0)
    seq_t lucas_curr = (seq_t)1;  // Lucas(1)
    
    for (idx_t i = 1; i < n; i++) {
        seq_t lucas_next = lucas_curr + lucas_prev;
        lucas_prev = lucas_curr;
        lucas_curr = lucas_next;
    }
    
    // lucas_curr = Lucas(n)
    // a(n) = Lucas(n) - 1 - (-1)^n
    if (n % 2 == 0) {
        // n even: a(n) = Lucas(n) - 2
        return lucas_curr - 2;
    } else {
        // n odd: a(n) = Lucas(n)
        return lucas_curr;
    }
}
