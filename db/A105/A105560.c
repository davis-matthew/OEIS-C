/*
 * A105560  a(1) = 1, and for n >= 2, a(n) = prime(bigomega(n)), where
 *          prime(n) = A000040(n) and bigomega(n) = A001222(n).
 * https://oeis.org/A105560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(bigomega(n)) for n≥2, a(1)=1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 8906 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* return the k‑th prime (k ≥ 1).  k is very small for the intended range. */
static seq_t prime_of_idx(idx_t k)
{
    if (k == 1) return 2;
    seq_t p = 3;
    idx_t cnt = 1;          /* prime 2 already counted */
    for (;;) {
        bool is_prime = true;
        for (seq_t d = 3; d * d <= p; d += 2) {
            if (p % d == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            ++cnt;
            if (cnt == k) return p;
        }
        p += 2;
    }
}

/* bigomega(n): total number of prime factors of n (counted with multiplicity) */
static idx_t bigomega_uint64(uint64_t x)
{
    idx_t cnt = 0;
    for (uint64_t i = 2; i * i <= x; ++i) {
        while (x % i == 0) {
            ++cnt;
            x /= i;
        }
    }
    if (x > 1) ++cnt;
    return cnt;
}

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    idx_t w = bigomega_uint64((uint64_t)n);
    return prime_of_idx(w);
}
