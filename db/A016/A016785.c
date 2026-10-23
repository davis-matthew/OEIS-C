/*
 * A016785  a(n) = (3*n + 1)^9.
 * https://oeis.org/A016785
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n + 1)^9
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 3, u64 n <= 45, u128 n <= 6370,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t before arithmetic to avoid C integer promotions. */
    seq_t base = (seq_t)n * (seq_t)3 + (seq_t)1;
    seq_t result = (seq_t)1;
    /* Raise to the ninth power by repeated multiplication. */
    for (int i = 0; i < 9; ++i)
        result *= base;
    return result;
}
