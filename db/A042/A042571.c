/*
 * A042571  Denominators of continued fraction convergents to sqrt(814).
 * https://oeis.org/A042571
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 22, u64 n <= 44, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of continued‑fraction convergents to sqrt(814).
   The convergents satisfy
      q(-1) = 0,  q(0) = 1,
      q(n)   = a(n) * q(n-1) + q(n-2)   for n ≥ 1
   where a(0) = 28 and the partial quotients repeat the 28‑term
   periodic block
      [1,1,7,1,1,1,5,18,1,5,2,1,1,4,1,1,2,5,1,18,5,1,1,1,7,1,1,56]
   (a(1) corresponds to the first term of this block).  This function
   returns the denominator q(n), which is a(0)=1, a(1)=1, … as listed
   in OEIS A042571. */
 
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* q0 */

    /* 28‑term repeating period after the initial a0 = 28 */
    static const seq_t period[28] = {
        (seq_t)1,(seq_t)1,(seq_t)7,(seq_t)1,(seq_t)1,(seq_t)1,(seq_t)5,(seq_t)18,
        (seq_t)1,(seq_t)5,(seq_t)2,(seq_t)1,(seq_t)1,(seq_t)4,(seq_t)1,(seq_t)1,
        (seq_t)2,(seq_t)5,(seq_t)1,(seq_t)18,(seq_t)5,(seq_t)1,(seq_t)1,(seq_t)1,
        (seq_t)7,(seq_t)1,(seq_t)1,(seq_t)56
    };

    seq_t q_prevprev = (seq_t)1; /* q0 */
    seq_t q_prev    = (seq_t)0;  /* q(-1) */
    seq_t q_curr    = (seq_t)0;
    idx_t idx       = 0;         /* position in period */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t ai = period[idx];
        q_curr = ai * q_prevprev + q_prev;
        /* shift denominators for next step */
        q_prev    = q_prevprev;
        q_prevprev = q_curr;
        idx++;
        if (idx == 28) idx = 0;
    }
    return q_curr;
}
