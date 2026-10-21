/*
 * A220946  Expansion of (1+2*x+2*x^2-x^3)/((1-x)*(1+x)*(1-3x^2)).
 * https://oeis.org/A220946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=2, a(2)=6; for n≥3 a(n) = a(n-1)+1 if n odd, a(n) =
 *            a(n-1)*3 if n even
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0U) return (seq_t)1;
    if (n == 1U) return (seq_t)2;
    if (n == 2U) return (seq_t)6;

    seq_t val = (seq_t)6;          /* a(2) */
    for (idx_t i = 3; i <= n; ++i) {
        if (i & 1U)                /* odd index */
            val = val + (seq_t)1;
        else                       /* even index */
            val = val * (seq_t)3;
    }
    return val;
}
