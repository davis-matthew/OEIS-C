/*
 * A070692  a(n) = n^7 mod 9.
 * https://oeis.org/A070692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^7 mod 9
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n mod 9 once (fits in seq_t). */
    seq_t base = (seq_t)n % 9;
    seq_t res  = 1 % 9;
    int i;
    /* Multiply 7 times, reducing modulo 9 at each step. */
    for (i = 0; i < 7; ++i)
        res = (res * base) % 9;
    return res;
}
