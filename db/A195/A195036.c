/*
 * A195036  Vertex number of a square spiral in which the length of the first
 *          two edges are the legs of the primitive Pythagorean triple [15, 8,
 *          17]. The edges of the spiral have length A195035.
 * https://oeis.org/A195036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n*(23*n+53) - (14*n+37)*(-1)^n + 37)/16
 * Range:     u8 n <= 1, u16 n <= 36, u32 n <= 9865, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* implemented without using negative intermediates */
seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;                       /* n in seq_t type */
    /* term = 2*n*(23*n + 53) */
    seq_t term = ns * ((seq_t)23 * ns + (seq_t)53);
    term = term * 2;
    seq_t num;
    if ((n & 1) == 0)                          /* n even */
        num = term - (seq_t)14 * ns;           /* subtract 14*n  */
    else                                       /* n odd  */
        num = term + (seq_t)14 * ns + (seq_t)74;/* add 14*n + 74  */
    return num / 16;                           /* integer division */
}
