/*
 * A060060  Third column of triangle A060058.
 * https://oeis.org/A060060
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+4,4)*(20*n^2+88*n+75)/15
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 62, u64 n <= 2629,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic for intermediate values so the
       calculation is exact for all n that fit into 64 bits
       and for arbitrary‑precision builds. */
    unsigned __int128 nn = n;

    /* binomial(n+4,4) = (n+1)(n+2)(n+3)(n+4)/24 */
    unsigned __int128 binom =
        (nn + 1) * (nn + 2) * (nn + 3) * (nn + 4) / 24;

    /* polynomial 20*n^2 + 88*n + 75 */
    unsigned __int128 poly = 20 * nn * nn + 88 * nn + 75;

    /* a(n) = binom * poly / 15 */
    unsigned __int128 num  = binom * poly;
    unsigned __int128 ans  = num / 15;

    return (seq_t)ans;
}
