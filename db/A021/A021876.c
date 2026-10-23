/*
 * A021876  Decimal expansion of 1/872.
 * https://oeis.org/A021876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = digit n of the decimal expansion of 1/872
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (a(0) is the first digit after the decimal point) */
seq_t a(idx_t n)
{
    seq_t r = (seq_t)1;        /* current remainder of division 1/872   */
    seq_t d = (seq_t)0;        /* last produced digit                  */
    const seq_t denom = (seq_t)872;   /* the divisor */

    /* Perform long division digit by digit up to index n */
    for (idx_t i = 0; i <= n; ++i) {
        r *= (seq_t)10;        /* bring next decimal digit into place   */
        d = r / denom;        /* current decimal digit                  */
        r = r % denom;        /* update remainder                        */
    }
    return d;
}
