/*
 * A007502  Les Marvin sequence: a(n) = F(n) + (n-1)*F(n-1), F() = Fibonacci
 *          numbers.
 * https://oeis.org/A007502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = F(n) + (n-1)*F(n-1), where F(n) is Fibonacci (F(0)=0,
 *            F(1)=1)
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 41, u64 n <= 85, u128 n <= 176,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t f_prev = (seq_t)0;
    seq_t f_curr = (seq_t)1;
    
    for (idx_t i = 2; i <= n; i++) {
        seq_t f_next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = f_next;
    }
    
    return f_curr + ((seq_t)n - 1) * f_prev;
}
