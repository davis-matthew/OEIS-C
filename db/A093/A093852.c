/*
 * A093852  a(n) = 10^(n-1) - 1 + n*floor(9*10^(n-1)/(n+1)).
 * https://oeis.org/A093852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 10^(n-1) - 1 + n*floor(9*10^(n-1)/(n+1))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(1) is the first listed term; n >= 1 */

seq_t a(idx_t n)
{
    /* The OEIS offset starts at 1.  For n < 1 the behaviour is irrelevant. */
    if (n < 1)
        return (seq_t)0;

    /* Compute 10^(n-1) iteratively.  For 64‑bit builds the product stays
       within range for all n whose result fits in uint64_t.  For arbitrary‑
       precision builds the multiplication is performed by the big‑int
       class. */
    seq_t pow10 = 1;                 /* 10^0 */
    for (idx_t i = 1; i < n; ++i)
        pow10 *= (seq_t)10;          /* 10^(i) */

    /* First part: 10^(n-1) - 1 */
    seq_t part1 = pow10 - (seq_t)1;

    /* Second part: n * floor(9*10^(n-1)/(n+1)) */
    seq_t floor_part = ((seq_t)9 * pow10) / ((seq_t)n + (seq_t)1);
    seq_t part2 = (seq_t)n * floor_part;

    return part1 + part2;
}
