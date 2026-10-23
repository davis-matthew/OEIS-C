/*
 * A046645  a(n) = log_2(A046644(n)); also the 2-adic valuation of A046644(n).
 * https://oeis.org/A046645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = sum_{p^e||n} (2*e - popcount(e)), where the sum runs over
 *            the prime power factors of n. This equals the 2‑adic valuation
 *            of A046644(n)
 * Range:     u8 n <= 9276, u16 n <= 9276, u32 n <= 9276, u64 n <= 9276,
 *            u128 n <= 9276, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static inline uint64_t popcount_u64(uint64_t x) {
    uint64_t c = 0;
    while (x) {
        x &= x - 1;  /* clear least significant 1 bit */
        ++c;
    }
    return c;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;                 /* no values below offset 1 */
    seq_t result = 0;
    uint64_t m = (uint64_t)n;            /* local copy for factorisation */

    for (uint64_t d = 2; d * d <= m; ++d) {
        if (m % d == 0) {
            uint64_t e = 0;
            while (m % d == 0) {
                m /= d;
                ++e;
            }
            /* f(e) = 2*e - popcount(e)  ( = e + v2(e!) ) */
            uint64_t pop = popcount_u64(e);
            result += (seq_t)(2 * e - pop);
        }
    }

    if (m > 1) {
        /* Remaining prime factor with exponent 1 */
        result += (seq_t)1;
    }
    return result;
}
