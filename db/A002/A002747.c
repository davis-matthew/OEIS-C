/*
 * A002747  Logarithmic numbers.
 * https://oeis.org/A002747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n*((n-1)*a(n-2)-(-1)^n), a(0)=0, a(1)=1
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    
    seq_t a0 = (seq_t)0;
    seq_t a1 = (seq_t)1;
    
    for (idx_t i = 2; i <= n; i++) {
        seq_t sign = ((i & 1) == 0) ? (seq_t)1 : (seq_t)(-1);
        seq_t ai = (seq_t)i * (((seq_t)(i - 1) * a0) - sign);
        a0 = a1;
        a1 = ai;
    }
    
    return a1;
}
