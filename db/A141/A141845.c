/*
 * A141845  a(n) = 5*a(n-1) + A047201(n), a(1) = 1. A047201 = numbers not
 *          divisible by 5: (1, 2, 3, 4, 6, 7, 8, 9, 11, ...).
 * https://oeis.org/A141845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) + A047201(n), a(1) = 1 A047201(n) = n +
 *            floor((n-1)/4) (the n‑th positive integer not divisible by 5)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;            /* undefined for n<1, return 0 as placeholder */

    seq_t val = (seq_t)1;            /* a(1) = 1 */

    for (idx_t i = (idx_t)2; i <= n; ++i)
    {
        /* A047201(i) = i + floor((i-1)/4) */
        seq_t add = (seq_t)i + (seq_t)((i - (idx_t)1) / (idx_t)4);
        val = val * (seq_t)5 + add;
    }

    return val;
}
