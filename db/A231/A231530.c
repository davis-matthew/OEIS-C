/*
 * A231530  Real part of Product_{k=1..n} (k+i), where i is the imaginary
 *          unit.
 * https://oeis.org/A231530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=1, a(1)=1, for n≥2: a(n) = (2*(n-1)+1)*a(n-1) –
 *            ((n-1)²+1)*a(n-2)
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)1;   /* a(1) */
    idx_t i;

    for (i = 2; i <= n; ++i) {
        /* i-1 fits in idx_t, promote to seq_t for arithmetic */
        seq_t im1 = (seq_t)(i - 1);

        seq_t coeff    = (seq_t)(im1 * 2 + 1);      /* 2*(n-1)+1 */
        seq_t subcoeff = (seq_t)(im1 * im1 + 1);    /* (n-1)^2+1 */

        seq_t an = coeff * a1 - subcoeff * a0;
        a0 = a1;
        a1 = an;
    }
    return a1;
}
