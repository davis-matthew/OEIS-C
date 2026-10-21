/*
 * A228292  a(n) = Sum_{k=1..8} n^k.
 * https://oeis.org/A228292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..8} n^k
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 15, u64 n <= 255, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum   = 0;
    seq_t power = 1;            /* power of n: starts at n^0 */
    for (idx_t i = 0; i < 8; ++i) {
        power *= (seq_t)n;      /* power = n^(i+1) */
        sum   += power;        /* add n^(i+1) to the sum */
    }
    return sum;
}
