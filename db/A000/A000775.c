/*
 * A000775  a(n) = n! * (n + 1 + 2*Sum_{k=1...n} 1/k).
 * https://oeis.org/A000775
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * (n + 1 + 2*Sum_{k=1...n} 1/k), Recurrence: a(0) = 1,
 *            a(n+1) = (n+1)*a(n) + (n+3)*n! for n >= 0
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    
    seq_t result = 1;
    seq_t factorial = 1;
    
    for (idx_t i = 0; i < n; i++) {
        seq_t i_seq = (seq_t)i;
        result = (i_seq + 1) * result + (i_seq + 3) * factorial;
        factorial = factorial * (i_seq + 1);
    }
    
    return result;
}
