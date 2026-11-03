/*
 * A182784  Primes of the form 2*n^4-1.
 * https://oeis.org/A182784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*A182783(n)^4-1 (Primes of the form 2*n^4‑1)
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 54, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 45 terms through n = 957 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

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
    /* Cache of previously found primes in the sequence */
    static seq_t *primes = NULL;
    static size_t primes_len = 0;
    static size_t primes_cap = 0;
    static idx_t next_k = 2;   /* smallest k to test next */

    if (n == 0) return 0;     /* not in OEIS range; safe guard */

    /* If we already have the requested prime, return it */
    if (primes_len >= (size_t)n) {
        return primes[n - 1];
    }

    /* One‑time allocation of the cache array */
    if (!primes) {
        primes_cap = 128;
        primes = mem_new(seq_t, primes_cap);
        if (!primes) return (seq_t)0;  /* allocation failure */
    }

    /* Generate primes of the form 2*k^4-1, starting from next_k */
    for (idx_t k = next_k; ; ++k) {
        seq_t k_big = (seq_t)k;
        seq_t k2    = k_big * k_big;   /* k^2 */
        seq_t k4    = k2 * k2;         /* k^4 */
        seq_t val   = k4 * 2 - 1;      /* 2*k^4-1 */

        if (isPrimeGeneric(val)) {
            /* Append to cache */
            if (primes_len >= primes_cap) {
                primes_cap <<= 1;
                primes = mem_resize(seq_t, primes, primes_cap);
                if (!primes) return (seq_t)0;  /* allocation failure */
            }
            primes[primes_len++] = val;
            next_k = k + 1;           /* Advance to next k for future calls */

            if (primes_len >= (size_t)n) {
                return val;           /* nth prime found */
            }
        }
    }
}
