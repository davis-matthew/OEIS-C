/*
 * A107601  Column 6 of array illustrated in A089574 and related to A034261.
 * https://oeis.org/A107601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n+1) * (1384 + 4683*(n+1)^2 + 2142*(n+1) + 2*(n+1)^6 +
 *            651*(n+1)^4 + 63*(n+1)^5 + 2835*(n+1)^3)) / 1680 (OEIS offset 0)
 * Range:     u8 none, u16 n <= 1, u32 n <= 16, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)(n + 1);          /* shift by one to match OEIS offset */

    seq_t m2 = m * m;
    seq_t m3 = m2 * m;
    seq_t m4 = m3 * m;
    seq_t m5 = m4 * m;
    seq_t m6 = m5 * m;

    seq_t poly =
        (seq_t)1384
        + (seq_t)4683 * m2
        + (seq_t)2142 * m
        + (seq_t)2      * m6
        + (seq_t)651    * m4
        + (seq_t)63     * m5
        + (seq_t)2835   * m3;

    return (m * poly) / (seq_t)1680;
}
