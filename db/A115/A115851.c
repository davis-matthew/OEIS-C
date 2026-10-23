/*
 * A115851  G.f. x^2*(-1+x+x^2)/((1-x)*(2*x-1)*(x+1)*(x^2+1)).
 * https://oeis.org/A115851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(2^n / 15) + ((n % 4 == 2 || n % 4 == 3) ? 1 : 0)
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t q = 0;         /* floor(2^n / 15) */
    unsigned r = 1;      /* 2^i mod 15, starts with 2^0 = 1 */

    /* iterate i = 1 .. n : double current value and update q,r */
    for (idx_t i = 1; i <= n; ++i) {
        unsigned tmp = r << 1;          /* r * 2  (tmp < 30) */
        unsigned add = tmp / 15;        /* floor(2 * r / 15)  (add is 0 or 1) */
        q = q * 2 + add;                /* q = floor( (2^i) / 15 ) */
        r = tmp - add * 15;             /* r = 2^i mod 15 */
    }

    /* add the correction term for n mod 4 == 2 or 3 */
    if ((n & 3U) == 2U || (n & 3U) == 3U)
        q = q + 1;

    return q;
}
