/*
 * A002249  a(n) = a(n-1) - 2*a(n-2) with a(0) = 2, a(1) = 1.
 * https://oeis.org/A002249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) - 2*a(n-2), a(0) = 2, a(1) = 1
 * Range:     i8 n <= 12, i16 n <= 28, i32 n <= 61, i64 n <= 124,
 *            i128 n <= 253, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)1;
    
    seq_t prev2 = (seq_t)2;
    seq_t prev1 = (seq_t)1;
    seq_t curr;
    
    for (idx_t i = 2; i <= n; i++) {
        curr = prev1 - (seq_t)2 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    
    return curr;
}
