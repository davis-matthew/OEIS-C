/*
 * A228294  a(n) = Sum_{k=1..10} n^k.
 * https://oeis.org/A228294
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..10} n^k
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 9, u64 n <= 84, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 0, a(1) = 10, ... with the closed form a(n) = (n^11-n)/(n-1) for n>1. */

seq_t a(idx_t n)
{
    seq_t sum   = 0;
    seq_t power = (seq_t)n;          /* n^1 */
    for (int k = 1; k <= 10; ++k) {
        sum += power;
        power *= (seq_t)n;           /* n^(k+1) */
    }
    return sum;
}
