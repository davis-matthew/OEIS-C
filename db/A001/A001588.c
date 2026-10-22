/*
 * A001588  a(n) = a(n-1) + a(n-2) - 1.
 * https://oeis.org/A001588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*Fibonacci(n) + 1
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    
    seq_t f0 = (seq_t)0;
    seq_t f1 = (seq_t)1;
    
    for (idx_t i = 2; i <= n; i++) {
        seq_t f_next = f0 + f1;
        f0 = f1;
        f1 = f_next;
    }
    
    return 2 * f1 + 1;
}
