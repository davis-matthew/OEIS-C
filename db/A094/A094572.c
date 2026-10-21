/*
 * A094572  Number of pairs of integers x, y (of either sign) with x^2 - y^2 =
 *          n.
 * https://oeis.org/A094572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*d(n) if n is odd; a(n) = 2*d(n/4) if n ≡ 0 mod 4;
 *            otherwise 0 (where d(m) is the number of positive divisors of m)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
static seq_t divisor_count_u64(uint64_t m) {
    uint64_t count = 1;
    uint64_t exp;
    if (m == 0) return 0; /* undefined, return 0 for safety */
    exp = 0;
    while ((m & 1ULL) == 0) { /* factor 2 */
        m >>= 1; exp++;
    }
    if (exp) count *= (exp + 1);
    for (uint64_t p = 3; (unsigned __int128)p * p <= m; p += 2) {
        if (m % p == 0) {
            exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
            }
            count *= (exp + 1);
        }
    }
    if (m > 1) count *= 2; /* remaining prime factor */
    return (seq_t)count;
}
seq_t a(idx_t n) {
    if (n < 1) return 0;
    if ((n & 1ULL) != 0ULL) {            /* n odd */
        seq_t d = divisor_count_u64((uint64_t)n);
        return (seq_t)2 * d;
    } else if ((n % 4) == 0) {           /* n ≡ 0 mod 4 */
        seq_t d = divisor_count_u64((uint64_t)(n / 4));
        return (seq_t)2 * d;
    } else {                              /* n ≡ 2 mod 4 */
        return 0;
    }
}
