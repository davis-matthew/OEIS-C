/*
 * A138478  Decimal expansion of 101/970299.
 * https://oeis.org/A138478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   decimal digit of 101/970299 at index n (0‑indexed)
 * Range:     u8 none, u16 n <= 2, u32 n <= 101, u64 n <= 101, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* period of the decimal expansion: 19602 = 2 * 3^4 * 11^2 */
    static const idx_t PERIOD = 19602ULL;
    /* denominator and numerator of the fraction */
    static const seq_t DENOM = 970299ULL;   /* 3^6 * 11^3 */
    static const seq_t NUM   = 101ULL;      /* numerator */

    idx_t i;
    /* reduce n modulo the period to avoid unnecessary iterations */
    idx_t k = n % PERIOD;

    seq_t rem   = NUM % DENOM;  /* initial remainder */
    seq_t digit = 0;

    /* perform k+1 steps of long division to reach the k‑th decimal digit */
    for (i = 0; i <= k; ++i) {
        rem   *= 10;
        digit  = rem / DENOM;
        rem    = rem % DENOM;
    }

    return digit;   /* a(n) is the decimal digit at position n */
}
