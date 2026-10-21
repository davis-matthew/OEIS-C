/*
 * A061001  Partial sums of the squares of the terms of A060999.
 * https://oeis.org/A061001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} A060999(k)^2 with A060999(k) = round(k^3/9) =
 *            (k^3 + 4) / 9 (integer division)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) is unused; return 0 for safety */
    if (n == (idx_t)0)
        return (seq_t)0;

    seq_t sum = (seq_t)0;
    for (idx_t k = (idx_t)1; k <= n; ++k)
    {
        seq_t term = ((seq_t)k * k * k + (seq_t)4) / (seq_t)9;
        sum += term * term;
    }
    return sum;
}
