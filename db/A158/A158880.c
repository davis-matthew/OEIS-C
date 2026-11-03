/*
 * A158880  Number of spanning trees in C_6 X P_n.
 * https://oeis.org/A158880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*U(n-1,3/2)^2*U(n-1,5/2)^2*U(n-1,3)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Validated: standard, pass, 27 terms through n = 90 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t
calc_U(idx_t m, seq_t two_x)            /* two_x = 2x for the recurrence */
{
    seq_t a0 = (seq_t)1;                /* U(0) = 1 */
    if (m == 0) return a0;
    seq_t a1 = two_x;                   /* U(1) = 2x */
    if (m == 1) return a1;
    for (idx_t i = 2; i <= m; ++i) {
        seq_t next = two_x * a1 - a0;   /* U(k+1)=2x*U(k)-U(k-1) */
        a0 = a1;
        a1 = next;
    }
    return a1;
}

seq_t a(idx_t n)
{
    /* n must be >= 1 (offset 1) */
    if (n < 1) return (seq_t)0;         /* undefined case, but safe */

    idx_t m = n - 1;                    /* index for the Chebyshev polynomials */

    /* coefficients 2x for the three needed polynomials */
    const seq_t coeff_3_2 = (seq_t)3;    /* 2*(3/2) = 3  */
    const seq_t coeff_5_2 = (seq_t)5;    /* 2*(5/2) = 5  */
    const seq_t coeff_3   = (seq_t)6;    /* 2*3     = 6  */

    seq_t u3  = calc_U(m, coeff_3_2);
    seq_t u5  = calc_U(m, coeff_5_2);
    seq_t u6  = calc_U(m, coeff_3);

    /* compute 6 * u3^2 * u5^2 * u6 */
    seq_t result = (seq_t)6;
    result *= u3;       /* multiply by u3 */
    result *= u3;       /* another u3 (squared) */
    result *= u5;       /* first u5 */
    result *= u5;       /* second u5 (squared) */
    result *= u6;       /* final u6 */

    return result;
}
