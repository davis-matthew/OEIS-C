/*
 * A170821  Let p = n-th prime; a(n) = smallest k >= 0 such that 4k == 3 mod
 *          p.
 * https://oeis.org/A170821
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = smallest k ≥ 0 such that 4k ≡ 3 (mod prime(n)) *
 *            Algorithm: For an odd prime p, inv4 = 4⁻¹ (mod p) can be
 *            obtained * with the closed form inv4 = (p+1)/4 if p ≡ 3 (mod 4),
 *            * inv4 = (3p+1)/4 if p ≡ 1 (mod 4). * Then a(n) = (3 * inv4) mod
 *            p
 * Range:     u8 n <= 68, u16 n <= 8482, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"

/* External linkage function as required */
seq_t a(idx_t n)
{
    /* Sequence is defined only for n ≥ 2; return 0 otherwise. */
    if (n < 2)
        return 0;

    /* Retrieve the n-th prime (1‑based). */
    /* get_prime returns a uint32_t, but promote to 64‑bit for safety. */
    uint64_t p = (uint64_t)get_prime(n);

    /* Compute the modular inverse of 4 modulo p. */
    uint64_t inv4;
    if ((p & 3ULL) == 3ULL)          /* p ≡ 3 (mod 4) */
        inv4 = (p + 1ULL) / 4ULL;
    else                            /* p ≡ 1 (mod 4) */
        inv4 = (3ULL * p + 1ULL) / 4ULL;

    /* Compute a(n) = (3 * inv4) mod p. */
    uint64_t res = (3ULL * inv4) % p;

    return (seq_t)res;
}
