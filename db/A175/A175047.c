/*
 * A175047  Write n in binary, then increase each run of 0's by one 0. a(n) is
 *          the decimal equivalent of the result.
 * https://oeis.org/A175047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = if n<2 then n else 2*(1 + 0^((n+2) mod 4))*a([n/2]) + n
 *            mod 2
 * Range:     u8 n <= 41, u16 n <= 2217, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)n;

    /* Recursive value on floor(n/2). */
    seq_t half = a(n >> 1);   /* n/2 */

    /* Compute factor: 1 + (0^((n+2) mod 4)) */
    /* 0^k is 1 when k == 0, otherwise 0 */
    seq_t factor = (((n + 2) % 4) == 0) ? (seq_t)2 : (seq_t)1;

    /* n mod 2 */
    seq_t mod2 = (seq_t)(n & 1);

    return (seq_t)(2 * factor * half + mod2);
}
