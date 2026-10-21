/*
 * A081250  Numbers k such that A081249(m)/m^2 has a local minimum for m = k.
 * https://oeis.org/A081250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(5 * 3^n / 4)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation is performed iteratively to avoid overflow: */
/*    let 3^n = 4*q + r  (0 ≤ r < 4).  Then a(n) = 5*q + r. */
/* At each step we update (q, r) from (q_prev, r_prev) by */
/*   3*(4*q_prev + r_prev) = 4*(3*q_prev) + 3*r_prev.  */
/* The new quotient and remainder are therefore: */
/*   q = 3*q_prev + floor(3*r_prev / 4),  r = 3*r_prev % 4.  */

seq_t a(idx_t n)
{
    seq_t q = 0;      /* floor of current value divided by 4 */
    seq_t r = 1;      /* remainder modulo 4 (3^0 = 1) */

    for (idx_t i = 0; i < n; ++i) {
        seq_t s = r * ((seq_t)3);    /* 0 ≤ s ≤ 9 */
        seq_t add = s / ((seq_t)4);  /* 0 ≤ add ≤ 2 */
        seq_t r_new = s % ((seq_t)4);/* 0 ≤ r_new ≤ 3 */

        q = q * ((seq_t)3) + add;
        r = r_new;
    }

    return q * ((seq_t)5) + r;       /* a(n) = floor(5*3^n/4) */
}
