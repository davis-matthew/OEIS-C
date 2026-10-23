/*
 * A154303  Primes of the form (1+2+...+m)/90 = A000217(m)/90.
 * https://oeis.org/A154303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy, bref
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A154303: Primes of the form (1+2+...+m)/90 = m(m+1)/(2·90) = m(m+1)/180.
    The sequence is finite: the only m ≤ 180 giving an integer prime are 35, 44, 179, 180.
    The implementation searches for m up to 200; this is sufficient for every
    supported seq_t (uint8_t, uint16_t, uint32_t, uint64_t, or arbitrary precision)
    because m(m+1)/180 ≤ 223 for m ≤ 200, which fits in a uint8_t. */

#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* sequence offset is 1, return 0 for n==0 */
        return 0;
    idx_t count   = 0;
    const idx_t MAX_M = 200;   /* upper limit safe for all seq_t sizes */
    for (idx_t m = 1; m <= MAX_M; ++m) {
        idx_t prod = m * (m + 1);              /* m·(m+1)  (fits in uint64_t) */
        if ((prod % 180u) != 0u)               /* must be divisible by 180 */
            continue;
        idx_t val64 = prod / 180u;             /* candidate prime (fits in idx_t) */
        seq_t val = (seq_t)val64;              /* cast to seq_t                */
        if ((seq_t)val64 != val)              /* overflow check for small seq_t */
            continue;
        if (is_prime_big(val)) {              /* primality test */
            ++count;
            if (count == n)                   /* n‑th prime found */
                return val;
        }
    }
    return 0;                                 /* no more terms beyond the known ones */
}
