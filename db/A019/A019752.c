/*
 * A019752  G.f.: 1/((1-4x)(1-11x)(1-12x)).
 * https://oeis.org/A019752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 23*a(n-1) - 132*a(n-2) + 4^n , with a(0)=1, a(1)=27
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)27;

    seq_t prev2 = (seq_t)1;              /* a(0) */
    seq_t prev1 = (seq_t)27;             /* a(1) */
    seq_t pow4  = (seq_t)4;              /* 4^1, ready for the first update */

    for (idx_t i = 2; i <= n; ++i) {
        /* Advance the power of 4: pow4 = 4^i */
        pow4 = pow4 * (seq_t)4;

        /* Compute a(i) using the 2‑term linear recurrence */
        seq_t cur = (seq_t)23 * prev1
                  - (seq_t)132 * prev2
                  + pow4;

        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
