/*
 * A070628  a(n) = n^5 mod 45.
 * https://oeis.org/A070628
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 mod 45
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 45 first to keep intermediate values small. */
    idx_t r = n % 45;
    seq_t m = (seq_t)r;          /* m is n mod 45, fits in seq_t */
    seq_t res = 1;               /* res = m^k mod 45 for k from 0 to 5 */

    /* Compute m^5 mod 45 by repeated multiplication. */
    for (int i = 0; i < 5; ++i)
        res = (res * m) % 45;

    return res;
}
