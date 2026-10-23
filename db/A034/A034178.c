/*
 * A034178  Number of solutions to n = a^2 - b^2, a > b >= 0.
 * https://oeis.org/A034178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = 0 if n % 4 == 2 a(n) = ⌊(d(n) + 1)/2⌋ if n % 4 == 1 or 3
 *            a(n) = ⌊(d(n/4) + 1)/2⌋ if n % 4 == 0 where d(k) is the number
 *            of positive divisors of k
 * Range:     u8 n <= 2000, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Count the positive divisors of the 64‑bit integer n.
   The result fits in seq_t because a(n) ≤ d(n) and the sequence
   values for n ≤ 10 000 are well below 256.                   */
static seq_t divisor_count_u64(idx_t n)
{
    idx_t cnt = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            ++cnt;                 /* divisor i */
            if (i != n / i) ++cnt; /* paired divisor n/i */
        }
    }
    return (seq_t)cnt;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* The OEIS offset is 1; n == 0 is out of range – return 0. */
    if (n == 0) return (seq_t)0;

    /* If n ≡ 2 (mod 4), there is no representation. */
    if ((n & 3ULL) == 2ULL) return (seq_t)0;

    /* Determine the argument for the divisor count. */
    idx_t m = ((n & 3ULL) == 0ULL) ? (n >> 2) : n;

    /* Compute the divisor count of m. */
    seq_t tau = divisor_count_u64(m);

    /* Return ⌊(tau + 1)/2⌋.  All arithmetic fits in seq_t for the
       ranges considered in the tests. */
    return (seq_t)((tau + 1ULL) / 2ULL);
}
