/*
 * A041628  Numerators of continued fraction convergents to sqrt(333).
 * https://oeis.org/A041628
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A041628: Numerators of convergents to sqrt(333).
   Recurrence:
   p_0 = 18,
   for n >= 1: p_n = a_n * p_{n-1} + p_{n-2},
   where a_n = 4  for odd n,
              a_n = 36 for even n. */

seq_t a(idx_t n)
{
    /* Handle explicit 0‑th term. */
    if (n == 0) {
        return (seq_t)18;
    }

    /* Initial values for the recurrence (p_{-1}=1, p_{-2}=0). */
    seq_t p_prev2 = (seq_t)0;     /* p_{k-2} */
    seq_t p_prev1 = (seq_t)1;     /* p_{k-1} */
    seq_t p_cur   = 0;            /* p_k */

    /* Loop from k=0 up to k=n, computing p_k. */
    for (idx_t k = 0; k <= n; ++k) {
        seq_t a_k;
        if (k == 0) {
            a_k = (seq_t)18;
        } else if ((k & (idx_t)1) == 1) {   /* odd k */
            a_k = (seq_t)4;
        } else {                           /* even k */
            a_k = (seq_t)36;
        }

        p_cur = a_k * p_prev1 + p_prev2;
        p_prev2 = p_prev1;
        p_prev1 = p_cur;
    }

    return p_cur;
}
