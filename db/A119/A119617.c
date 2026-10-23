/*
 * A119617  Integers of the form c(n)/b(n) where c(n+1)=c(n)+(n+1)^4 with
 *          c(0)=1 and b(n+1)=b(n)+(n+1)^2 with b(0)=1.
 * https://oeis.org/A119617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (30*n*(n-1)-3*(2*n-1)*(-1)^n+5)/8
 * Range:     u8 n <= 3, u16 n <= 131, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t N   = (seq_t)n;
    seq_t num = (seq_t)30 * N * (N - 1) + (seq_t)5;
    seq_t t   = (seq_t)3 * ((seq_t)2 * N - (seq_t)1);
    if ((n & 1) == 0)           /* n even: subtract t */
        num -= t;
    else                         /* n odd:   add t */
        num += t;
    return num / (seq_t)8;
}
