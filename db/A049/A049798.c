/*
 * A049798  a(n) = (1/2)*Sum_{k = 1..n} T(n,k), array T as in A049800.
 * https://oeis.org/A049798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=2..floor((n+1)/2)} ((n+1) mod k)
 * Range:     u8 n <= 69, u16 n <= 1113, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n + 1;               /* m = n+1 */
    seq_t sum = 0;                 /* running total */

    for (idx_t k = 2; k <= m / 2; ++k) {
        sum += (seq_t)(m % k);     /* add the remainder (n+1) mod k */
    }
    return sum;
}
