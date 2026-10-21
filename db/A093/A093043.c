/*
 * A093043  a(n) = Jacobsthal(n)*Fibonacci(n-1).
 * https://oeis.org/A093043
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A093043: a(n) = Jacobsthal(n) * Fibonacci(n-1)
   Jacobsthal recursion: J(0)=0, J(1)=1, J(n)=J(n-1)+2*J(n-2)
   Fibonacci recursion:  F(0)=0, F(1)=1, F(n)=F(n-1)+F(n-2)
   For n=0 and n=1 the product is 0.  The algorithm runs in O(n) time
   and uses only integer arithmetic on seq_t, which is safe for
   arbitrary‑precision or 64‑bit builds. */
seq_t a(idx_t n)
{
    if (n <= 1u) /* a(0)=a(1)=0 */
        return (seq_t)0;

    /* Fibonacci(n-1) */
    seq_t f_prev2 = 0;   /* F(0) */
    seq_t f_prev1 = 1;   /* F(1) */
    /* loop to compute F(n-1) */
    for (idx_t i = 2; i <= n - 1; ++i) {
        seq_t f_cur = f_prev1 + f_prev2;
        f_prev2 = f_prev1;
        f_prev1 = f_cur;
    }
    seq_t fib_nm1 = f_prev1;   /* F(n-1) */

    /* Jacobsthal(n) */
    seq_t j_prev2 = 0;   /* J(0) */
    seq_t j_prev1 = 1;   /* J(1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t j_cur = j_prev1 + (j_prev2 << 1);  /* J(i) = J(i-1)+2*J(i-2) */
        j_prev2 = j_prev1;
        j_prev1 = j_cur;
    }
    seq_t jacob_n = j_prev1;  /* J(n) */

    return jacob_n * fib_nm1;
}
