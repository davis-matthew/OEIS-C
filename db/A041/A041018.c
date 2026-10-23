/*
 * A041018  Numerators of continued fraction convergents to sqrt(13).
 * https://oeis.org/A041018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 29, u64 n <= 61, u128 n <= 123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to √13.
   The partial quotients of √13 are: 3; [1,1,1,1,6] (period 5).
   Let p(-1)=1 and p(0)=3.  For n≥1,
      p(n) = c(n) * p(n‑1) + p(n‑2)
   where c(n)=1 for n≡1,2,3,4 (mod 5) and c(n)=6 for n≡0 (mod 5).
   The sequence a(n) is p(n).  (OEIS A041018) */

seq_t a(idx_t n)
{
    static const seq_t period[5] = {
        (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)6
    };

    if (n == 0u) return (seq_t)3;

    /* p(-1) = 1,  p(0) = 3 */
    seq_t p_minus1 = (seq_t)1;
    seq_t p_curr   = (seq_t)3;

    for (idx_t k = 1; k <= n; ++k) {
        seq_t coeff = period[(k - 1) % 5];
        seq_t next  = coeff * p_curr + p_minus1;
        p_minus1 = p_curr;
        p_curr   = next;
    }
    return p_curr;
}
