/*
 * A000776  a(n) = n! * (1 + 2*Sum_{k=1..n} 1/k).
 * https://oeis.org/A000776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * (1 + 2*Sum_{k=1..n} 1/k), using recurrence a(0)=1,
 *            a(n+1) = (n+1)*a(n) + 2*n!
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    
    seq_t result = 1;  // a(0)
    seq_t fact = 1;    // 0!
    
    for (idx_t i = 1; i <= n; i++) {
        result = i * result + 2 * fact;
        fact *= i;
    }
    
    return result;
}
