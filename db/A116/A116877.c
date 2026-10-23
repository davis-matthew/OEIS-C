/*
 * A116877  Generalized Catalan numbers C(2,7;n)=C(7,2;n).
 * https://oeis.org/A116877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A116877 (Generalized Catalan numbers C(2,7;n))
   Recurrence:
     a(0) = 1, a(1) = 1;
     For n >= 2:
       a(n) = (56 * Cat_{n-2} * 14^{n-2} - 22 a(n-1) - 7 a(n-2)) / 3
   where Cat_k is the k‑th Catalan number defined by
     Cat_0 = 1
     Cat_{k+1} = Cat_k * 2*(2k+1)/(k+2).
*/
seq_t a(idx_t n)
{
    /* special cases */
    if (n == 0 || n == 1) {
        return (seq_t)1;
    }

    seq_t a_prev2 = (seq_t)1;    /* a(0)   */
    seq_t a_prev1 = (seq_t)1;    /* a(1)   */
    seq_t cat     = (seq_t)1;    /* Cat_{i-2}  at loop start   */
    seq_t pow14   = (seq_t)1;    /* 14^{i-2}   at loop start   */

    for (idx_t i = 2; i <= n; ++i) {
        /* RHS: 56 * Cat_{i-2} * 14^{i-2} */
        seq_t rhs = (seq_t)56 * cat * pow14;

        /* a(i) = (rhs - 22*a(i-1) - 7*a(i-2))/3 */
        seq_t val   = rhs - (seq_t)22 * a_prev1 - (seq_t)7 * a_prev2;
        seq_t a_curr = val / 3;

        if (i == n) {
            return a_curr;
        }

        /* prepare values for next iteration */
        cat = (cat * ((seq_t)2 * (2 * (i - 2) + 1))) / (seq_t)i;
        pow14 *= (seq_t)14;

        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }

    return (seq_t)1;  /* unreachable */
}
