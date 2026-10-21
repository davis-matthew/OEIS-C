/*
 * A211390  The minimum cardinality of an n-qubit unextendible product basis.
 * https://oeis.org/A211390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + 1 if n is odd; a(n) = n + 2 if n = 2 (mod 4) or n =
 *            4; a(n) = n + 3 if n = 8; a(n) = n + 4 otherwise
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1) != 0)                  /* n odd */
        return (seq_t)n + (seq_t)1;
    if (n == 4)                        /* special case n = 4 */
        return (seq_t)n + (seq_t)2;
    if ((n & 3) == 2)                  /* n ≡ 2 (mod 4) */
        return (seq_t)n + (seq_t)2;
    if (n == 8)                        /* special case n = 8 */
        return (seq_t)n + (seq_t)3;
    /* all remaining even n that are not 2 mod 4, not 4, not 8 */
    return (seq_t)n + (seq_t)4;
}
