/*
 * A166656  Totally multiplicative sequence with a(p) = 7p-1 for prime p.
 * https://oeis.org/A166656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Π_{p^e||n} (7p-1)^e (multiplicative, a(1)=1)
 * Range:     u8 n <= 5, u16 n <= 31, u32 n <= 511, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square‑root:  floor(sqrt(n)) for 0 <= n < 2^64 */
static idx_t isqrt_u64(idx_t n) {
    if (n < 2) return n;
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        if (mid <= n / mid)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n) {
    if (n == 1) return 1;

    seq_t result = 1;
    idx_t temp = n;

    /* factor 2 separately */
    if ((temp & 1) == 0) {
        int exp = 0;
        while ((temp & 1) == 0) {
            temp >>= 1;
            ++exp;
        }
        seq_t base = (seq_t)7 * 2 - 1;   /* 13 */
        seq_t pow = 1;
        while (exp--) pow *= base;
        result *= pow;
    }

    /* factor odd primes up to sqrt(temp) */
    for (idx_t p = 3; p <= isqrt_u64(temp); p += 2) {
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            seq_t base = (seq_t)7 * p - 1;
            seq_t pow = 1;
            while (exp--) pow *= base;
            result *= pow;
        }
    }

    /* remaining prime factor, if any */
    if (temp > 1) {
        seq_t base = (seq_t)7 * temp - 1;
        result *= base;
    }

    return result;
}
