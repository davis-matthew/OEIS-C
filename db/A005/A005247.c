/*
 * A005247  a(n) = 3*a(n-2) - a(n-4), a(0)=2, a(1)=1, a(2)=3, a(3)=2.
 *          Alternates Lucas (A000032) and Fibonacci (A000045) sequences for
 *          even and odd n.
 * https://oeis.org/A005247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = L(n) if n even, F(n) if n odd (Lucas and Fibonacci
 *            sequences)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 47, u64 n <= 93, u128 n <= 185,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t a_val, b_val;
    
    if (n % 2 == 0) {
        // Lucas numbers: L(0)=2, L(1)=1, L(n) = L(n-1) + L(n-2)
        a_val = (seq_t)2;
        b_val = (seq_t)1;
    } else {
        // Fibonacci numbers: F(0)=0, F(1)=1, F(n) = F(n-1) + F(n-2)
        a_val = (seq_t)0;
        b_val = (seq_t)1;
    }
    
    for (idx_t i = 2; i <= n; i++) {
        seq_t temp = a_val + b_val;
        a_val = b_val;
        b_val = temp;
    }
    
    return (n == 0) ? a_val : b_val;
}
