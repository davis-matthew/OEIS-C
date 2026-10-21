/*
 * A082290  Expansion of (1+x+x^2)/((1+x^2)*(1+x)^4*(1-x)^5).
 * https://oeis.org/A082290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n^4+108*n^3+666*n^2+1620*n+1251 +
 *            (4*n^3+54*n^2+236*n+333)*(-1)^n - 48*(-1)^((6*n-1+(-1)^n)/4)) /
 *            1536
 * Range:     u8 none, u16 n <= 5, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute powers of n */
    seq_t n_sq   = (seq_t)n * (seq_t)n;          /* n^2 */
    seq_t n_cube = n_sq * (seq_t)n;              /* n^3 */
    seq_t n_four = n_cube * (seq_t)n;            /* n^4 */

    /* main polynomial components */
    seq_t A = 6 * n_four + 108 * n_cube + 666 * n_sq
            + 1620 * (seq_t)n + 1251;
    seq_t B = 4 * n_cube + 54 * n_sq + 236 * (seq_t)n + 333;

    /* parity of n and control signs */
    int sign_n = (n & 1) ? -1 : 1;         /* (-1)^n  */

    /* parity of exponent e = (6*n-1+(-1)^n)/4 */
    // e parity can be derived without full division:
    // If n even: e = 3*n/2, its parity = (n/2)%2
    // If n odd : e = (3*n-1)/2, its parity = 1 - (n/2)%2
    idx_t half = n >> 1;                  /* n/2 */
    int  e_parity = ((n & 1) == 0) ? (int)(half & 1)
                                    : (int)(((half + 1) & 1));
    int sign_e = (e_parity == 0) ? 1 : -1; /* (-1)^e */

    /* assemble numerator */
    seq_t numer = A;
    if (sign_n == 1)
        numer += B;
    else
        numer -= B;

    if (sign_e == 1)
        numer -= 48;
    else
        numer += 48;

    /* final division (exact) */
    return numer / 1536;
}
