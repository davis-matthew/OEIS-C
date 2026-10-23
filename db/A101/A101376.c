/*
 * A101376  a(n) = n^2*(n^3 - n^2 + n + 1)/2.
 * https://oeis.org/A101376
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(n^3 - n^2 + n + 1)/2
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 96, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t ns = (seq_t)n;
    seq_t n2 = ns * ns;          /* n^2 */
    seq_t n3 = n2 * ns;          /* n^3 */
    seq_t t  = n3 - n2 + ns + 1; /* n^3 - n^2 + n + 1 */
    return (n2 * t) / 2;         /* n^2 * t / 2 */
}
