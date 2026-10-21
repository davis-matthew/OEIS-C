/*
 * A070640  a(n) = n^6 mod 17.
 * https://oeis.org/A070640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6 mod 17
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* All operations are on values < 17, so a 64‑bit or smaller type is enough. */
    unsigned r = n % 17;                 /* r in [0,16] */
    unsigned r2 = (r * r) % 17;          /* r^2 mod 17 */
    unsigned r4 = (r2 * r2) % 17;        /* r^4 mod 17 */
    unsigned r6 = (r4 * r2) % 17;        /* r^6 mod 17 */
    return (seq_t)r6;
}
