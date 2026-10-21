/*
 * A070642  a(n) = n^6 mod 19.
 * https://oeis.org/A070642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6 mod 19
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 19 before performing the exponentiation.
       Since 19 is small, intermediate values stay within
       the range of seq_t. */
    seq_t mod      = (seq_t)19;
    seq_t n_mod    = (seq_t)(n % 19);
    seq_t result   = (seq_t)1;

    for (int i = 0; i < 6; ++i)          /* compute n_mod^6 mod 19 */
        result = (result * n_mod) % mod;

    return result;
}
