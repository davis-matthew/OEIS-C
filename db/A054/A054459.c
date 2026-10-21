/*
 * A054459  A001333(n), n >= 1, convolved with itself.
 * https://oeis.org/A054459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 21, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 4*a(n–1) – 2*a(n–2) – 4*a(n–3) – a(n–4),
   a(0)=1, a(1)=6, a(2)=23, a(3)=76,  n ≥ 0 */
seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0ULL) return (seq_t)1;
    if (n == 1ULL) return (seq_t)6;
    if (n == 2ULL) return (seq_t)23;
    if (n == 3ULL) return (seq_t)76;

    /* previous four values of the sequence:
       prev4 = a(n-4), prev3 = a(n-3), prev2 = a(n-2), prev1 = a(n-1)   */
    seq_t prev4 = (seq_t)1;
    seq_t prev3 = (seq_t)6;
    seq_t prev2 = (seq_t)23;
    seq_t prev1 = (seq_t)76;

    seq_t cur = 0;
    for (idx_t i = 4; i <= n; ++i)
    {
        cur = 4 * prev1 - 2 * prev2 - 4 * prev3 - prev4;
        /* shift the window forward */
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
