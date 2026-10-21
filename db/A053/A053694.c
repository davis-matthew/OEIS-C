/*
 * A053694  Number of self-conjugate 5-core partitions of n.
 * https://oeis.org/A053694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A053694 – Number of self‑conjugate 5‑core partitions of n.
 *
 *  The value a(n) is b(n+1) where b is multiplicative:
 *
 *      – b(2^e) = b(5^e) = 1
 *      – if p ≡ 1 or 5 (mod 8):  b(p^e) = e + 1
 *      – if p ≡ 3 or 7 (mod 8):  b(p^e) = (1 + (-1)^e)/2
 *          (i.e. 1 when e is even, 0 when e is odd)
 *
 *  The implementation factors n+1 with the provided helper "factor_u64"
 *  and multiplies the contributions from each prime power.
 */

#include <stdint.h>
#include <stddef.h>          /* size_t */
#include "../utils/factor_u64.h"

/* forward declaration of seq_t and idx_t (supplied by the build) */

/* Compute the sequence value. */
seq_t a(idx_t n)
{
    /* Convert index to the factorised argument m = n + 1. */
    uint64_t m = (uint64_t)n + 1ULL;

    /* Handle the pathological overflow case: n = UINT64_MAX -> m = 0. */
    if (m == 0)
        return (seq_t)0;          /* a(UINT64_MAX) is undefined – return 0 */

    /* Storage for prime factors and their exponents. 64 slots are
       sufficient for a 64‑bit integer (max 64 distinct prime factors). */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t  cnt = 0;

    /* Factorise m.  The helper writes the list of primes, their
       exponents, and the count into the supplied arrays. */
    factor_u64(m, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t factor;

        if (p == 2 || p == 5) {
            factor = (seq_t)1;            /* rule: b(2^e) = b(5^e) = 1 */
        } else {
            uint64_t mod8 = p & 7ULL;     /* p mod 8 */

            if (mod8 == 1 || mod8 == 5) { /* p ≡ 1 or 5 (mod 8) */
                factor = (seq_t)(e + 1);  /* b(p^e) = e + 1 */
            } else {                     /* p ≡ 3 or 7 (mod 8) */
                factor = ((e & 1ULL) == 0) ? (seq_t)1 : (seq_t)0;
                /* b(p^e) = 1 if e even, 0 if e odd */
            }
        }

        /* If any factor is 0, the product becomes 0, no need to
           continue further. */
        if (factor == 0)
            return (seq_t)0;

        result *= factor;
    }

    return result;
}
