/*
 * A123378  Values x of the solutions (x,y) of the Diophantine equation
 *          5*(X-Y)^4 - 2*X*Y = 0 with X >= Y.
 * https://oeis.org/A123378
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computation of OEIS A123378.
   It implements the recurrence with auxiliary sequences 
   c(n) and d(n):
      c(0)=0,   c(1)=3,   c(n)=38*c(n-1)-c(n-2)
      d(0)=1,   d(1)=19,  d(n)=38*d(n-1)-d(n-2)
      a(n) = c(n)*(1+d(n))
   The function works for any non‑negative n and returns
   the exact value when it fits in the target integer type.
*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return (seq_t)60; /* 3*(1+19) */

    /* c(n-1), c(n-2)   and   d(n-1), d(n-2) */
    seq_t c_prev   = (seq_t)3;    /* c(1) */
    seq_t c_prev2  = (seq_t)0;    /* c(0) */
    seq_t d_prev   = (seq_t)19;   /* d(1) */
    seq_t d_prev2  = (seq_t)1;    /* d(0) */

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t c_next = (seq_t)38 * c_prev - c_prev2;
        seq_t d_next = (seq_t)38 * d_prev - d_prev2;

        c_prev2 = c_prev;
        c_prev  = c_next;
        d_prev2 = d_prev;
        d_prev  = d_next;
    }

    return c_prev * ((seq_t)1 + d_prev);
}
