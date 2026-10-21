/*
 * A070510  a(n) = n^3 mod 48.
 * https://oeis.org/A070510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 mod 48
 * Range:     u8 n <= 8, u16 n <= 292, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t mod = (seq_t)48;               /* modulus as seq_t */
    seq_t nn  = (seq_t)n;                /* convert index to seq_t */
    seq_t r   = nn * nn;                 /* n^2 */
    r = r % mod;                         /* reduce modulo 48 */
    r = r * nn;                          /* n^3 */
    r = r % mod;                         /* final reduction */
    return r;
}
