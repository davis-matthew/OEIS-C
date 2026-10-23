/*
 * A041526  Numerators of continued fraction convergents to sqrt(280).
 * https://oeis.org/A041526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 41, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A041526: Numerators of continued fraction convergents to sqrt(280).
   Partial quotients after the first term are periodic with block
   [1, 2, 1, 2, 1, 32].
   Convergent numerators a(n) satisfy:
       a(-2) = 0, a(-1) = 1,
       a(n) = a_n * a(n-1) + a(n-2),
   where a_n = 16 for n = 0, and otherwise a_n is taken from the
   periodic block above.  The function returns a(n) for any n ≥ 0. */
seq_t a(idx_t n)
{
    /* Periodic part of the continued fraction for sqrt(280). */
    static const seq_t period[6] = { 1, 2, 1, 2, 1, 32 };

    seq_t p_nm2 = 0;          /* a(-2)   */
    seq_t p_nm1 = 1;          /* a(-1)   */
    seq_t p_n;                /* a(n)    */

    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t ai = (i == 0) ? (seq_t)16 : period[(i - 1) % 6];
        p_n   = ai * p_nm1 + p_nm2;
        p_nm2 = p_nm1;
        p_nm1 = p_n;
    }
    return p_nm1;
}
