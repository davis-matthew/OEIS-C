/*
 * A094921  a(n) = (6^n-1)/5 mod n.
 * https://oeis.org/A094921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (6^n - 1)/5 mod n (OEIS A094921)
 * Range:     u8 n <= 263, u16 n <= 8976, u32 n <= 8976, u64 n <= 8976,
 *            u128 n <= 8976, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute 6^n mod (5*n) using binary exponentiation, then return (result-1)/5.
 * The intermediate values are kept below 5*n <= 5*10^4 for n ≤ 10000, so
 * 64‑bit arithmetic with 128‑bit cast for the product is safe. */
seq_t a(idx_t n) {
    /* Offset is 1; a(1) = 0.  For safety, if n==0 return 0. */
    if (n == 0) return 0;

    uint64_t M = 5ULL * (uint64_t)n;          /* modulus 5*n */
    uint64_t result = 1ULL % M;              /* 6^0 mod M */
    uint64_t base   = 6ULL % M;              /* 6^1 mod M */
    idx_t e = n;

    while (e > 0) {
        if (e & 1ULL)
            result = (uint64_t)(((__uint128_t)result * base) % M);
        base = (uint64_t)(((__uint128_t)base * base) % M);
        e >>= 1;
    }

    uint64_t val = (result - 1ULL) / 5ULL;    /* (6^n-1)/5, already < n */
    return (seq_t)val;                       /* cast to sequence type */
}
