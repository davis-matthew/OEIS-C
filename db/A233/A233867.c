/*
 * A233867  a(n) = |{0 < m < 2*n: m is a square with 2*n - 1 - phi(m) prime}|,
 *          where phi(.) is Euler's totient function (A000010).
 * https://oeis.org/A233867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{0 < m < 2*n : m = k^2, m is a square, and 2*n - 1 -
 *            φ(m) is prime}|
 * Range:     u8 n <= 569, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9883 (2026-11-01)
 * Helpers:   is_prime, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/totient.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t two_n = (seq_t)n << 1;   /* 2*n */
    seq_t limit = two_n - 1;       /* 2*n - 1 (used for p computation) */

    for (idx_t k = 1; ; ++k) {
        seq_t m = (seq_t)k * (seq_t)k;
        if (m >= two_n) {
            break;   /* m must be strictly less than 2*n */
        }
        seq_t phi_k = totient(k);      /* φ(k) */
        seq_t phi_m = (seq_t)k * phi_k;   /* φ(k²) = k * φ(k) */
        seq_t p = limit - phi_m;          /* 2*n - 1 - φ(m) */
        if (p > 0 && is_prime((idx_t)p)) {
            ++count;
        }
    }
    return count;
}
