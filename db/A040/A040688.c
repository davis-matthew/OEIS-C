/*
 * A040688  Continued fraction for sqrt(715).
 * https://oeis.org/A040688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   Continued fraction for sqrt(715) a(0) = 26 For n >= 1, the terms
 *            repeat with period 10: [1, 2, 1, 5, 5, 5, 1, 2, 1, 52]
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) is a special case */
    if (n == 0)
        return (seq_t)26U;

    /* periodic part after the first term */
    static const seq_t period[10] = {1U, 2U, 1U, 5U, 5U, 5U, 1U, 2U, 1U, 52U};

    /* (n-1) because the first periodic element corresponds to n = 1 */
    idx_t idx = (n - 1) % (idx_t)10;  /* safe modulo on unsigned 64‑bit */
    return period[idx];
}
