/*
 * A167527  n^5 mod 49.
 * https://oeis.org/A167527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 mod 49
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n modulo 49 */
    idx_t m = n % (idx_t)49;
    seq_t val = (seq_t)m;          /* first power: m^1 */
    /* multiply by m four more times to reach m^5 */
    val = (val * (seq_t)m) % (seq_t)49;
    val = (val * (seq_t)m) % (seq_t)49;
    val = (val * (seq_t)m) % (seq_t)49;
    val = (val * (seq_t)m) % (seq_t)49;
    return val;
}
