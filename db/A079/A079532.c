/*
 * A079532  a(n) = floor(n - sqrt(n)) - phi(n).
 * https://oeis.org/A079532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 208, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Validated: standard, pass, 102 terms through n = 994 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = floor(n - sqrt(n)) - φ(n)
 * Computes a(n) for n ≥ 1.
 * Uses an integer–only square‑root implementation and the pre‑provided
 * totient routine from utils/phi.h.
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Integer square root for 64-bit unsigned values. */
static inline unsigned long long isqrt(unsigned long long n)
{
    unsigned long long res = 0;
    unsigned long long bit = 1ULL << 62;          /* highest bit <= sqrt(2^64-1) */
    while (bit > n) bit >>= 2;
    for (; bit; bit >>= 2) {
        if (n >= res + bit) {
            n  -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
    }
    return res;
}

static seq_t phi(idx_t m)
{
    seq_t result = (seq_t)m;
    idx_t d = 2;
    for (; d * d <= m; ++d) {
        if (m % d == 0) {
            while (m % d == 0) m /= d;
            result -= result / (seq_t)d;
        }
    }
    if (m > 1) result -= result / (seq_t)m;
    return result;
}

/* Integer square root: floor(sqrt(n)) for 0 ≤ n < 2^64. */
static idx_t isqrt_u64(idx_t n)
{
    idx_t res = 0;
    idx_t bit = 1ULL << 62;          /* largest power of 4 ≤ 2^64  */

    while (bit > n) bit >>= 2;
    while (bit != 0) {
        if (n >= res + bit) {
            n -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* floor(sqrt(n)) */
    idx_t s = isqrt_u64(n);
    seq_t seqs = (seq_t)s;
    seq_t seqn = (seq_t)n;

    /* floor(n - sqrt(n)) */
    seq_t floor_term = seqn - seqs;
    if (seqs * seqs != seqn)        /* n not a perfect square */
        floor_term -= 1;

    /* φ(n) from utils/phi.h */
    seq_t phi_val = phi(n);         /* the function is defined in phi.h */

    return floor_term - phi_val;
}
