/*
 * A158414  a(n) = 961*n + 1.
 * https://oeis.org/A158414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 961*n + 1
 * Range:     u8 none, u16 n <= 68, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast the constant to seq_t to avoid unintended promotion */
    return (seq_t)961 * (seq_t)n + (seq_t)1;
}
