/*
 * A042694  Numerators of continued fraction convergents to sqrt(877).
 * https://oeis.org/A042694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 23, u64 n <= 49, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for the numerators of the continued‐fraction convergents to sqrt(877).
 *
 *  a(0) = 29
 *  a(n) = cf[(n‑1) % 15] * a(n‑1) + a(n‑2)   (n ≥ 1)
 *
 *  where the periodic continued‑fraction terms after the initial 29 are
 *  cf = {1,1,1,1,2,4,1,1,4,2,1,1,1,1,58}.
 *  The period length is 15. */

seq_t a(idx_t n)
{
    if (n == 0)
        return 29;

    static const unsigned short cf[15] =
        {1, 1, 1, 1, 2, 4, 1, 1, 4, 2, 1, 1, 1, 1, 58};

    const unsigned period = 15;
    seq_t p_prev2 = 1;   /* p_{-1} */
    seq_t p_prev1 = 29;  /* p_0   */

    for (idx_t k = 1; k <= n; ++k) {
        unsigned short ak = cf[(k - 1) % period];
        seq_t tmp = (seq_t)ak * p_prev1 + p_prev2;
        p_prev2 = p_prev1;
        p_prev1 = tmp;
    }

    return p_prev1;
}
