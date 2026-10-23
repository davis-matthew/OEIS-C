/*
 * A041774  Numerators of continued fraction convergents to sqrt(408).
 * https://oeis.org/A041774
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A041774: Numerators of continued‑fraction convergents to √408.
   Recurrence:   p(-2)=0, p(-1)=1;
   For n≥0, let c(n)=
         20          if n==0
         5           if n is odd
         40          if n is even (n≥2)
   Then    p(n)=c(n)*p(n-1)+p(n-2).  The returned value is p(n). */

seq_t a(idx_t n)
{
    /* Base cases handled in the loop with initial p(-2)=0, p(-1)=1. */
    seq_t p_prev2 = (seq_t)0;   /* p(-2) */
    seq_t p_prev1 = (seq_t)1;   /* p(-1) */
    seq_t p_curr  = (seq_t)0;   /* would become p(n) */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t c;
        if (k == 0)
            c = (seq_t)20;
        else
            c = (k & 1) ? (seq_t)5 : (seq_t)40;  /* odd →5, even→40 */

        p_curr = c * p_prev1 + p_prev2;
        p_prev2 = p_prev1;
        p_prev1 = p_curr;
    }

    return p_prev1;  /* p(n) */
}
