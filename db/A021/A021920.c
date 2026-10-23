/*
 * A021920  Decimal expansion of 1/916.
 * https://oeis.org/A021920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^{n+1} / 916) - 10 * floor(10^n / 916)
 * Range:     u8 n <= 3, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) gives the n-th decimal digit (0‑indexed) of 1/916 */
seq_t a(idx_t n)
{
    const seq_t denom = 916;
    seq_t r = 1;     /* initial remainder = 1 */
    seq_t d = 0;     /* digit to return */

    for (idx_t i = 0; i <= n; ++i) {
        r *= 10;
        d = r / denom; /* next digit   */
        r %= denom;    /* update remainder */
    }
    return d;
}
