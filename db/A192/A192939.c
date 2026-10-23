/*
 * A192939  Constant term of the reduction by x^2->x+1 of the polynomial
 *          p(n,x)=(x+2)(x+4)...(x+2n).
 * https://oeis.org/A192939
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: A(0)=1,B(0)=0.
   For i = 1..n: 
       A_new = 2*i*A + B
       B_new = A + (2*i+1)*B
   The required value a(n) is the constant term A(n). */
seq_t a(idx_t n)
{
    seq_t A = (seq_t)1;
    seq_t B = (seq_t)0;
    for (idx_t i = (idx_t)1; i <= n; ++i) {
        seq_t ii = (seq_t)i;
        seq_t two_i = ii + ii;
        seq_t newA = two_i * A + B;
        seq_t newB = A + (two_i + (seq_t)1) * B;
        A = newA;
        B = newB;
    }
    return A;
}
