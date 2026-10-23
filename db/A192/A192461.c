/*
 * A192461  Coefficient of x in the reduction by x^2->x+1 of the polynomial
 *          p(n,x) defined below in Comments.
 * https://oeis.org/A192461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for the coefficient of x in the reduction of
   p(n,x) described in OEIS A192461.  Let
     c_0 = 0,  a_0 = 1     (reduced form of p(0,x)=x)
   For i = 1,2,…, let
       c_i = i * c_{i-1} + (i+1) * a_{i-1}
       a_i = (2*i+1) * a_{i-1} + (i+1) * c_{i-1}
   Then a(n) = a_{n-1}.  The function below implements this
   recurrence exactly using the types seq_t and idx_t supplied
   by the build. */
seq_t a(idx_t n)
{
    seq_t c = 0;
    seq_t a = 1;          /* coefficient of x for p(0,x)=x */
    if (n <= 1)
        return a;        /* a(1) = 1 */

    for (idx_t i = 1; i < n; ++i) {
        seq_t i_seq    = (seq_t)i;
        seq_t next_c   = i_seq * c + (i_seq + 1) * a;
        seq_t next_a   = (2 * i_seq + 1) * a + (i_seq + 1) * c;
        c = next_c;
        a = next_a;
    }
    return a;
}
