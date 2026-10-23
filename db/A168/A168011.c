/*
 * A168011  a(n) = Sum of all numbers of divisors of all numbers < (n+1)^2.
 * https://oeis.org/A168011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{(n+1)^2-1} d(k) where d(k) is the number of
 *            divisors of k. The divisor summatory function satisfies D(m) =
 *            Σ_{i=1}^m ⌊m / i⌋, so a(n) = D((n+1)^2−1)
 * Range:     u8 n <= 6, u16 n <= 84, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n must be ≥1; for n==0 return 0 as a defensive fallback */
    if (n == 0) return 0;

    /* Compute M = (n+1)^2 − 1, the largest integer to include. */
    idx_t np1 = n + 1;
    idx_t M = np1 * np1 - 1;

    seq_t total = 0;
    idx_t i = 1;

    /* Group equal quotients floor(M / i) to achieve O(√M) complexity. */
    while (i <= M) {
        idx_t q    = M / i;
        idx_t last = M / q;                 /* last index where floor(M / i) == q */
        seq_t cnt  = (seq_t)(last - i + 1);  /* how many i give this q   */
        total += (seq_t)q * cnt;             /* add contribution           */
        i = last + 1;
    }

    return total;
}
