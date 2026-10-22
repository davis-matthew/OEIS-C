/*
 * A003422  Left factorials: !n = Sum_{k=0..n-1} k!.
 * https://oeis.org/A003422
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Sum_{k=0..n-1} k! (left factorial)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t factorial = 1;
    
    for (idx_t k = 0; k < n; k++) {
        result += factorial;
        factorial *= (seq_t)(k + 1);
    }
    
    return result;
}
