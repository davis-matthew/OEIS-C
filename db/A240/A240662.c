/*
 * A240662  Least k such that 7^k == -1 (mod prime(n)), or 0 if no such k
 *          exists.
 * https://oeis.org/A240662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Least k such that 7^k ≡ -1 (mod prime(n)), or 0 if no such k
 *            exists. The least k, if it exists, such that prime(n) divides
 *            7^k + 1
 * Range:     u8 n <= 97, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, get_prime, powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>     /* size_t */
#include <stdint.h>
#include <stdbool.h>

/* Helper functions provided by the build system. */
#include "../utils/get_prime.h"    /* uint32_t get_prime(idx_t k); */
#include "../utils/factor_u64.h"   /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */
#include "../utils/powmod64.h"    /* uint64_t powmod64(uint64_t base, uint64_t exp, uint64_t mod); */

/* Compute a(n).  idx_t is unsigned 64‑bit.  seq_t is an
   unsigned type chosen by the build (8, 16, 32, 64 or 128 bits). */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for n < 1 */

    /* 1. Obtain the n‑th prime. */
    uint32_t prime32 = get_prime(n);   /* get_prime returns uint32_t */
    uint64_t p = (uint64_t)prime32;

    /* 2. Special case: prime 2.  7 ≡ 1 (mod 2) and -1 ≡ 1, so k=1. */
    if (p == 2ULL)
        return (seq_t)1;

    /* 3. Compute the multiplicative order of 7 modulo p. */
    uint64_t phi = p - 1ULL;            /* φ(p) for prime p */

    /* Factorization of phi: store prime factors and exponents. */
    uint64_t fac_pr[20];
    uint64_t fac_ex[20];
    size_t fac_cnt = 0;
    factor_u64(phi, fac_pr, fac_ex, &fac_cnt);

    /* Start with the full exponent and try to reduce it. */
    uint64_t order = phi;
    for (size_t i = 0; i < fac_cnt; ++i) {
        uint64_t q = fac_pr[i];
        /* Remove this factor as much as possible. */
        while ((order % q) == 0ULL) {
            uint64_t candidate = order / q;
            if (powmod64(7ULL, candidate, p) == 1ULL)
                order = candidate;
            else
                break;
        }
    }

    /* 4. If the order is odd, 7^k cannot be -1 modulo p. */
    if ((order & 1ULL) != 0ULL)
        return (seq_t)0;

    /* 5. Candidate k = order/2; verify that it yields -1. */
    uint64_t k = order >> 1;  /* order / 2 */
    if (powmod64(7ULL, k, p) == (p - 1ULL))
        return (seq_t)k;
    else
        return (seq_t)0;
}
