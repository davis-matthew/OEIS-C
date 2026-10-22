/*
 * A007489  a(n) = Sum_{k=1..n} k!.
 * https://oeis.org/A007489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   a(n) = Sum_{k=1..n} k!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t factorial = 1;
    
    for (idx_t k = 1; k <= n; k++) {
        factorial *= (seq_t)k;
        result += factorial;
    }
    
    return result;
}
