/*
 * A185091  The smallest positive noncomposite q such that 2n-1 = 2p+q for
 *          some positive noncomposite p.
 * https://oeis.org/A185091
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 784, u16 n <= 10002, u32 n <= 10002, u64 n <= 10002,
 *            u128 n <= 10002, big any n
 * Validated: standard, pass, 109 terms through n = 9284 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A185091.
   a(n) is the smallest positive non‑composite q such that
   2n–1 = 2p + q with p, q non‑composite (1 or prime).  */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 2; for smaller n the
       returned value is irrelevant. */
    if (n < 2)
        return (seq_t)0;

    /*  N = 2n – 1  (odd).  Cast n to seq_t first to avoid overflow
        in the intermediate product. */
    seq_t ns = (seq_t)n;
    seq_t N  = (seq_t)2 * ns - (seq_t)1;

    /* Try q = 1 first.  It works iff p = (N–1)/2 is 1 or prime. */
    seq_t p0 = (N - (seq_t)1) / (seq_t)2;
    if (p0 == (seq_t)1 || isPrimeGeneric(p0))
        return (seq_t)1;

    /* Scan odd values for q, testing primality on the fly. */
    for (seq_t q = (seq_t)3; q < N; q += (seq_t)2)
    {
        if (!isPrimeGeneric(q))
            continue;

        seq_t p = (N - q) / (seq_t)2;
        if (p == (seq_t)0) /* p would become < 1 */
            break;

        if (p == (seq_t)1 || isPrimeGeneric(p))
            return q;
    }

    /* By definition an appropriate q always exists; this is a safe
       fallback that should never be reached. */
    return (seq_t)1;
}
