/*
 * A070601  n^5 mod 17.
 * https://oeis.org/A070601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^5) mod 17
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 17 first */
    seq_t m = (seq_t)n % 17;
    seq_t res = 1;
    /* Compute m^5 mod 17 */
    for (idx_t i = 0; i < 5; ++i)
    {
        res = (res * m) % 17;
    }
    return res;
}
