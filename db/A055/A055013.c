/*
 * A055013  Sum of 4th powers of digits of n.
 * https://oeis.org/A055013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = sum_{k>0} (digit_k)^4 where digit_k = floor(n/10^k)
 *            -10*floor(n/10^(k+1)) Recurrence: a(10n + k) = a(n) + k^4 (0 ≤ k
 *            ≤ 9)
 * Range:     u8 n <= 3, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the sum of the fourth powers of the decimal digits of n. */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    while (n > 0) {
        unsigned d = n % 10;            /* 0 … 9 */
        seq_t d2 = (seq_t)d * (seq_t)d; /* d^2  (≤ 81)   */
        seq_t d4 = d2 * d2;             /* d^4  (≤ 6561) */
        sum += d4;
        n /= 10;
    }
    return sum;
}
