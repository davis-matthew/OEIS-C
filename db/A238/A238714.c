/*
 * A238714  Final divisor of A238529(n).
 * https://oeis.org/A238714
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 10001, u32 n <= 10001, u64 n <= 10001,
 *            u128 n <= 10001, big any n
 * Validated: standard, pass, 93 terms through n = 6876 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A238714.
 *
 * The function repeatedly replaces the current number `cur` by
 * `cur % sopfr(cur)` until either the remainder becomes zero
 * or `cur` equals 1.  The final divisor returned is the last
 * value of `sopfr(cur)` used in the step that caused the stop.
 *
 * sopfr(n) = sum of prime factors of `n` counted with multiplicity.
 * It is computed by trial division up to sqrt(n).
 *
 * Complexity: O(√n) per call, which is fast enough for n up to 10 000
 * and reasonably large `n`.  The algorithm works for any value
 * that fits into `seq_t` (the build‑dependent integer type).
 */

#include <stdint.h>

static inline seq_t sopfr(idx_t x)
{
    if (x < 2) return 0;
    seq_t sum = 0;
    /* handle factor 2 */
    while ((x & 1ULL) == 0) {
        sum += 2;
        x >>= 1;
    }
    /* odd factors */
    for (idx_t d = 3; d * d <= x; d += 2) {
        while (x % d == 0) {
            sum += d;
            x /= d;
        }
    }
    if (x > 1) sum += (seq_t)x;
    return sum;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;           /* undefined for n<2, return 0 */
    idx_t cur = n;
    seq_t last_d = 0;
    while (1) {
        seq_t d = sopfr(cur);
        seq_t r = (seq_t)cur % d;
        if (r == 0) return d;     /* remainder becomes zero */
        cur = (idx_t)r;
        last_d = d;
        if (cur == 1) return last_d;  /* chain reached 1 */
    }
}
