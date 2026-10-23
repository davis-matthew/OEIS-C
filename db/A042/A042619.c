/*
 * A042619  Denominators of continued fraction convergents to sqrt(838).
 * https://oeis.org/A042619
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the denominator of the nth convergent to √838
   (OEIS A042619).  The continued‑fraction expansion of √838 is
   [28;1,18,3,6,9,2,28,2,9,6,3,18,1,56] with period 14, so a(0)=28 and for
   n≥1 a(n)=period[(n‑1) mod 14].  The convergents satisfy
   q₋₂=1, q₋₁=0, qₙ=aₙ·qₙ₋₁+qₙ₋₂.  This function returns qₙ. */
seq_t a(idx_t n)
{
    static const seq_t period[14] = {1,18,3,6,9,2,28,2,9,6,3,18,1,56};
    seq_t q_prevprev = 1; /* q₋₂  */
    seq_t q_prev    = 0;  /* q₋₁  */
    seq_t q_curr    = 0;
    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t ai = (i == 0) ? 28ULL : period[(i - 1) % 14ULL];
        q_curr = ai * q_prev + q_prevprev;
        q_prevprev = q_prev;
        q_prev     = q_curr;
    }
    return q_curr;
}
