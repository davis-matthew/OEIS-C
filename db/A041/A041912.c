/*
 * A041912  Numerators of continued fraction convergents to sqrt(478).
 * https://oeis.org/A041912
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 19, u64 n <= 42, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of numerators of convergents to sqrt(478).
   The sequence is defined by the recurrence for convergents of a
   simple continued fraction.  Let
      a0 = ⌊√478⌋ = 21,
   and initialise
      m0 = 0,  d0 = 1,  a0 = 21.
   Then for k ≥ 0 compute
      mk+1 = dk * ak - mk,
      dk+1 = (478 - mk+1²) / dk,
      ak+1 = ⌊(a0 + mk+1)/dk+1⌋.
   The numerators Pk of the convergents are obtained from
      P−2 = 0,  P−1 = 1,
      Pk   = ak * Pk−1 + Pk−2 .
   Your function returns Pn (= a(n)).  */
seq_t a(idx_t n)
{
    const idx_t D = 478;          /* constant D */
    const idx_t a0 = 21;          /* floor(sqrt(D)) */

    /* Base convergents */
    seq_t prev2 = 0;          /* P_{k-2} */
    seq_t prev1 = 1;          /* P_{k-1} */
    seq_t curr;

    /* Convergent for k = 0 */
    if (n == 0)
        return (seq_t)a0;

    /* State for continued fraction expansion */
    idx_t m = 0;
    idx_t d = 1;
    idx_t a = a0;

    for (idx_t k = 0; k <= n; ++k)
    {
        if (k == 0) {
            curr = (seq_t)a0 * prev1 + prev2;   /* P0 = a0*1 + 0 = a0 */
        } else {
            /* generate next partial quotient */
            idx_t m_next = d * a - m;
            idx_t d_next = (D - m_next * m_next) / d;
            idx_t a_next = (a0 + m_next) / d_next;

            m = m_next;
            d = d_next;
            a = a_next;

            curr = (seq_t)a * prev1 + prev2;   /* Pk = ak*Pk-1 + Pk-2 */
        }

        if (k == n)
            return curr;

        /* shift for next iteration */
        prev2 = prev1;
        prev1 = curr;
    }

    /* Should never reach here */
    return 0;
}
