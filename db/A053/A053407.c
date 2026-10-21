/*
 * A053407  Traditional type sizes (in typographic points) for printing.
 * https://oeis.org/A053407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = n+6 for n < 7 a(n) = 2n for 7 ≤ n < 10 a(n) = 2n+1 for n
 *            = 10 a(n) = 2n+2 for n = 11 a(n) = 12·(n-9) for n ≥ 12 (all
 *            operations performed in seq_t to preserve full precision)
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n < 7  → 6,7,8,9,10,11,12 */
    if (n < 7)
        return (seq_t)n + (seq_t)6;

    /* 7 ≤ n < 10 → 14,16,18 */
    if (n < 10)
        return (seq_t)n * (seq_t)2;

    /* n = 10 → 21 */
    if (n == 10)
        return (seq_t)n * (seq_t)2 + (seq_t)1;

    /* n = 11 → 24 */
    if (n == 11)
        return (seq_t)n * (seq_t)2 + (seq_t)2;

    /* n ≥ 12 → arithmetic progression with difference 12 */
    return (seq_t)n * (seq_t)12 - (seq_t)108;
}
