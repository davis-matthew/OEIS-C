/*
 * A098808  a(n) = 2^(n + 11) - 11.
 * https://oeis.org/A098808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n + 11) – 11
 * Range:     u8 none, u16 n <= 5, u32 n <= 21, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 2037, a(1) = 4085, … */

seq_t a(idx_t n)
{
    /* Compute 2^(n+11) by repeated multiplication by 2 */
    seq_t val = 1;
    idx_t steps = n + 11;
    for (idx_t i = 0; i < steps; ++i) {
        val <<= 1;             /* multiply by 2 */
    }
    val -= (seq_t)11;          /* subtract 11 */
    return val;
}
