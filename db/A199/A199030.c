/*
 * A199030  a(n) = 10*11^n-1.
 * https://oeis.org/A199030
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*11^n-1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 9, a(n) = 11*a(n-1)+10,  n≥1 */
seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)9;

    seq_t res = (seq_t)9;
    for (idx_t i = 1; i <= n; ++i)
        res = res * (seq_t)11 + (seq_t)10;

    return res;
}
