/*
 * A093121  A Jacobsthal Fibonacci product.
 * https://oeis.org/A093121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((2^n + 2*(-1)^n)/3) * Fib(n+1) Compute via the
 *            recurrences: J(0)=1, J(1)=0, J(k)=J(k-1)+2*J(k-2) (Jacobsthal
 *            variant) F(0)=0, F(1)=1, F(k)=F(k-1)+F(k-2) (Fibonacci) Then
 *            a(n)=J(n)*F(n+1)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute J(n):  J(0)=1, J(1)=0, J(k)=J(k-1)+2*J(k-2) */
    seq_t j_prev2 = (seq_t)1;   /* J(0) */
    seq_t j_prev1 = (seq_t)0;   /* J(1) */
    seq_t j_val;

    if (n == (idx_t)0) {
        j_val = j_prev2;        /* J(0) */
    } else if (n == (idx_t)1) {
        j_val = j_prev1;        /* J(1) */
    } else {
        /* Iterate from k=2 up to n */
        idx_t k;
        for (k = (idx_t)2; k <= n; ++k) {
            seq_t new_j = j_prev1 + (seq_t)2 * j_prev2;
            j_prev2 = j_prev1;
            j_prev1 = new_j;
        }
        j_val = j_prev1; /* J(n) */
    }

    /* Compute Fibonacci F(n+1) */
    seq_t f_prev2 = (seq_t)0;   /* F(0) */
    seq_t f_prev1 = (seq_t)1;   /* F(1) */
    seq_t f_val;
    idx_t m = n + (idx_t)1;     /* we need F(m) = F(n+1) */

    if (m == (idx_t)0) {
        f_val = f_prev2;
    } else if (m == (idx_t)1) {
        f_val = f_prev1;
    } else {
        idx_t i;
        for (i = (idx_t)2; i <= m; ++i) {
            seq_t new_f = f_prev1 + f_prev2;
            f_prev2 = f_prev1;
            f_prev1 = new_f;
        }
        f_val = f_prev1;   /* F(m) = F(n+1) */
    }

    /* Return the product */
    return j_val * f_val;
}
