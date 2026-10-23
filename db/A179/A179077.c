/*
 * A179077  a(n) is the residue ((2^p - 2)/p) mod p, where p is the n-th
 *          prime.
 * https://oeis.org/A179077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((2^p - 2)/p) mod p, where p is the n-th prime
 * Range:     u8 n <= 55, u16 n <= 6675, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A179077 with arbitrary-precision seq_t
 * and 64-bit intermediate arithmetic. */
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/powmod64.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;                /* offset starts at 1 */

    /* nth prime (fits in uint32_t for the required range) */
    uint32_t p32 = get_prime(n);              /* p < 2^32 */
    uint64_t p  = (uint64_t)p32;

    /* Compute 2^p modulo p^2.  p^2 < 2^64 for p up to the 10,000th prime. */
    uint64_t pp = p * p;
    uint64_t r  = powmod64(2ULL, (uint64_t)p, pp);  /* 2^p mod p^2 */

    /* (r - 2) may underflow if r < 2; correct for that by adding p^2. */
    uint64_t diff;
    if (r >= 2ULL) {
        diff = r - 2ULL;
    } else {
        diff = r + pp - 2ULL;   /* r + p^2 - 2 is the non‑negative remainder */
    }

    /* quotient is < p, fits in seq_t. */
    uint64_t quotient = diff / p;
    return (seq_t)quotient;
}
