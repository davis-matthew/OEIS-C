/*
 * A103458  a(n) = 7^n + 1 - 0^n.
 * https://oeis.org/A103458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 7^n + 1 for n ≥ 1, a(0) = 1 (since 0^0 = 1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 7*a(n‑1) – 6 with a(1) = 8 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)8;

    seq_t val = (seq_t)8;          /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        val = (val * (seq_t)7) - (seq_t)6;
    }
    return val;
}
