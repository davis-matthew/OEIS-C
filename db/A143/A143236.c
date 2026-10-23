/*
 * A143236  a(n) = A000005(n) * A006218(n).
 * https://oeis.org/A143236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = d(n) * Σ_{k=1..n} ⌊n/k⌋ where d(n) is the number of
 *            divisors of n (A000005)
 * Range:     u8 n <= 17, u16 n <= 479, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the number of divisors d(n) by trial division.
   Works for any 64‑bit n; for n up to a few thousand it
   is instantaneous. */
static seq_t divisor_count(idx_t n)
{
    if (n == 0) return 0;
    seq_t d = 1;
    uint64_t m = (uint64_t)n;

    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            d *= (seq_t)(e + 1);
        }
    }
    if (m > 1) d *= (seq_t)2;
    return d;
}

/* Compute Σ_{k=1..n} ⌊n/k⌋ in O(√n) time using the
   usual interval grouping trick. */
static seq_t floor_sum(idx_t n)
{
    seq_t sum = 0;
    idx_t i = 1;
    while (i <= n) {
        idx_t q    = n / i;
        idx_t next = n / q + 1;                /* first i for which floor changes */
        seq_t cnt  = (seq_t)(next - i);         /* number of i in [i, next) */
        sum += cnt * (seq_t)q;
        i = next;
    }
    return sum;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t d  = divisor_count(n);
    seq_t s  = floor_sum(n);
    return d * s;
}
