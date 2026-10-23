/*
 * A169204  Number of reduced words of length n in Coxeter group on 47
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^27 = I.
 * https://oeis.org/A169204
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1 a(1)=47 a(2)=2162 a(3)=99452 a(4)=4574792 a(5)=210440432
 *            a(n)=46*a(n-1) for n≥6 (This recurrence was derived from the
 *            rational generating function of A169204)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the initial values explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)47;
    if (n == 2) return (seq_t)2162;
    if (n == 3) return (seq_t)99452;
    if (n == 4) return (seq_t)4574792;
    if (n == 5) return (seq_t)210440432;

    /* for n ≥ 6, use fast exponentiation: a(n) = 210440432 · 46^(n‑5) */
    idx_t exp = n - 5;
    seq_t result = (seq_t)210440432;
    seq_t base   = (seq_t)46;

    while (exp > 0) {
        if (exp & 1ULL)      /* odd exponent: multiply result by base */
            result *= base;
        exp >>= 1ULL;        /* shift exponent */
        if (exp)             /* avoid useless squaring after last step */
            base *= base;
    }
    return result;
}
