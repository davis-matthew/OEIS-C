/*
 * A144600  Christoffel word of slope 2/11.
 * https://oeis.org/A144600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-13) (periodic with period 13)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The first 13 terms (for n = 0..12) */
    static const seq_t vals[13] = {
        0, /* a(0) */
        0, /* a(1) */
        0, /* a(2) */
        0, /* a(3) */
        0, /* a(4) */
        0, /* a(5) */
        1, /* a(6) */
        0, /* a(7) */
        0, /* a(8) */
        0, /* a(9) */
        0, /* a(10) */
        0, /* a(11) */
        1  /* a(12) */
    };
    /* Return the term corresponding to n modulo 13 */
    return vals[n % 13];
}
