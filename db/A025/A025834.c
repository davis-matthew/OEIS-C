/*
 * A025834  Expansion of 1/((1-x^3)*(1-x^4)*(1-x^12)).
 * https://oeis.org/A025834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^2 + 4*n + 8*(n+6)*(1-(n mod 3)) +
 *            6*(n+10)*(1+((n+3) mod 4)))/288)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Use signed 64‑bit intermediates to accommodate negative terms safely */
    long long n_ll = (long long)n;
    long long sq   = n_ll * n_ll;                    /* n^2 */
    long long term2 = 4 * n_ll;                       /* 4*n */

    /* 8*(n+6)*(1-(n mod 3)) */
    long long term3 = 8 * (n_ll + 6);
    long long mod3 = n_ll % 3;
    if (mod3 == 2) term3 = -term3;   /* 1-(n mod 3) = -1 */
    else if (mod3 == 1) term3 = 0;    /* 1-(n mod 3) = 0 */
    /* otherwise (mod3==0) term3 stays positive */

    /* 6*(n+10)*(1+((n+3) mod 4)) */
    long long mod4 = (n_ll + 3) % 4;  /* 0..3 */
    long long term4 = 6 * (n_ll + 10) * (1 + mod4);

    long long numerator = sq + term2 + term3 + term4;
    return (seq_t)(numerator / 288); /* integer division truncates toward zero */
}
