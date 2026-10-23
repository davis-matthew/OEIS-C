/*
 * A103495  Multiplicative suborder of 9 (mod n) = sord(9, n).
 * https://oeis.org/A103495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = minimum e ≥ 1 such that 9^e ≡ ±1 (mod n), or 0 if no such e. */

#include <stdint.h>

/* Helper: compute φ(n) for n ≥ 1. */
static inline idx_t phi(idx_t n) {
    idx_t result = n;
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            result -= result / p;
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) result -= result / m;
    return result;
}

seq_t a(idx_t n) {
    /* Handle the offset and trivial cases. */
    if (n <= 1) return 0;
    if (n % 3 == 0) return 0;        /* 9 shares a factor 3 → no ±1 can occur. */

    seq_t mod = (seq_t)n;
    /* Pre‑compute the totient to bound the search. */
    idx_t limit = phi(n);

    seq_t current = (seq_t)9 % mod;   /* 9^1 mod n */
    for (idx_t e = 1; e <= limit; ++e) {
        if (current == (seq_t)1 || current == mod - 1) {
            return (seq_t)e;
        }
        current = (current * (seq_t)9) % mod;
    }
    return 0;   /* No exponent up to φ(n) yields ±1. */
}
