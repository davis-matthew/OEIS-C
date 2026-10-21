/*
 * A070626  a(n) = n^5 mod 43.
 * https://oeis.org/A070626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 mod 43
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* First reduce n modulo 43 to avoid large intermediate results. */
    idx_t m = n % 43;
    seq_t base = (seq_t)m;          /* base = n mod 43 */
    seq_t result = 1;               /* result = base^0 */

    /* Compute base^5 modulo 43 by repeated multiplication. */
    for (int i = 0; i < 5; ++i)
        result = (result * base) % 43;

    return result;                  /* guaranteed to be < 43 */
}
