/*
 * A041584  Numerators of continued fraction convergents to sqrt(310).
 * https://oeis.org/A041584
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 23, u64 n <= 47, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A041584: Numerators of continued fraction convergents to sqrt(310).
   Let b[n] be the nth partial quotient of sqrt(310) with
   b[0] = 17 and for n>=1, b[n] repeats the period
   [1,1,1,1,5,3,1,2,1,3,5,1,1,1,1,34].
   The sequence is given by a(0)=b[0] and a(n)=b[n]*a(n-1)+a(n-2) for n>=1. */
seq_t a(idx_t n)
{
    /* period of partial quotients after the initial 17 */
    static const seq_t period[16] = {
        1,1,1,1,5,3,1,2,1,3,5,1,1,1,1,34
    };

    seq_t p_nm2 = 0;    /* p_{n-2} */
    seq_t p_nm1 = 1;    /* p_{n-1} */

    for (idx_t i = 0; i <= n; ++i) {
        seq_t bi = (i == 0) ? 17 : period[(i - 1) % 16];
        seq_t p_n = bi * p_nm1 + p_nm2;
        p_nm2 = p_nm1;
        p_nm1 = p_n;
    }
    return p_nm1;
}
