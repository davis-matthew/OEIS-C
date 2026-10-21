/*
 * A053469  a(n) = n*6^(n-1).
 * https://oeis.org/A053469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*6^(n-1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Singly‑indexed sequence: n >= 1 */
    seq_t power = 1;        /* 6^(n-1)   */
    idx_t i;

    /* Multiply by 6, (n-1) times */
    for (i = 1; i < n; ++i) {
        power *= (seq_t)6;
    }

    /* Multiply by n and return */
    return power * (seq_t)n;
}
