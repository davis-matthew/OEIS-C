/*
 * A087120  Smallest numbers having in binary representation exactly n maximal
 *          groups of consecutive zeros.
 * https://oeis.org/A087120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=0, a(2)=10, a(n)=4*a(n-1)+2 for n≥3. The terms are
 *            defined by a recurrence starting at n=2. This implementation
 *            uses only integer operations and works for any seq_t size
 *            (fixed‑width or arbitrary precision)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)0;
    if (n == 2u) return (seq_t)10;

    /* Iterate the recurrence for n ≥ 3 */
    seq_t value = (seq_t)10;          /* a(2) */
    for (idx_t i = 3u; i <= n; ++i) {
        value = (seq_t)4 * value + (seq_t)2;
    }
    return value;
}
