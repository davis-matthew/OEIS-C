/*
 * A089090  a(n) is the smallest composite number coprime to n.
 * https://oeis.org/A089090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (smallest prime p that does not divide n)^2 (AEIS
 *            A089090)
 * Range:     u8 n <= 9722, u16 n <= 9722, u32 n <= 9722, u64 n <= 9722,
 *            u128 n <= 9722, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* ---- small prime test, fast enough for p <= a few thousand ---- */
static inline int is_prime_u64(uint64_t x) {
    if (x < 2) return 0;
    if (x % 2 == 0) return x == 2;
    for (uint64_t i = 3; i * i <= x; i += 2) {
        if (x % i == 0) return 0;
    }
    return 1;
}

/* --------------------------------------------------------------- */
seq_t a(idx_t n) {
    /* n is 1-indexed, assume n >= 1 */
    uint64_t N = (uint64_t)n;          /* cast the index for arithmetic */
    uint64_t p = 2;                    /* candidate prime */

    for (;;) {
        /* skip non-primes */
        if (is_prime_u64(p) && (N % p != 0)) {
            /* p is the smallest prime coprime to n */
            return (seq_t)p * (seq_t)p;  /* square in seq_t domain */
        }
        ++p;
    }
}
