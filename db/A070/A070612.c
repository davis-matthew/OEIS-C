/*
 * A070612  n^5 mod 28.
 * https://oeis.org/A070612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 mod 28
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 28 first to keep intermediate results small */
    seq_t modVal = (seq_t)28;
    seq_t x   = ((seq_t)n) % modVal;          /* n mod 28 */
    seq_t x2  = (x * x) % modVal;             /* n^2 mod 28 */
    seq_t x4  = (x2 * x2) % modVal;           /* n^4 mod 28 */
    seq_t res = (x4 * x) % modVal;            /* n^5 mod 28 */
    return res;
}
