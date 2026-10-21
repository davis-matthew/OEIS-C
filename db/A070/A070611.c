/*
 * A070611  n^5 mod 27.
 * https://oeis.org/A070611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 mod 27
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 27 first to keep the intermediate results small. */
    seq_t nn = (seq_t)n % 27;

    /* Compute nn^5 mod 27 by repeated multiplication. */
    seq_t res = 1;
    for (int i = 0; i < 5; ++i)
        res = (res * nn) % 27;

    return res;
}
