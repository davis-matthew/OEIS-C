/*
 * A216311  The prime ending in 7 is the only prime in a decade.
 * https://oeis.org/A216311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, changed
 * Formula:   a(n) is the n-th prime p ≡ 7 (mod 10) such that * p-6, p-4, and
 *            p+2 are composite (10n+1, 10n+3, 10n+9 are composite)
 * Range:     u8 n <= 2, u16 n <= 743, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 61 terms through n = 9728 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
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
    /* Special case: OEIS offset is 1, so n must be ≥1. */
    if (n == 0) return 0;

    /* Static cache of found terms. */
    static seq_t *cache   = NULL;
    static size_t cache_sz = 0;
    static size_t cache_cap = 0;

    /* The last candidate index k (so candidate = 10*k + 7). */
    static idx_t last_k = 0;

    /* Max value that a seq_t can hold when it fits into 64 bits. */
    static const uint64_t MAX_SEQ_VAL = (sizeof(seq_t) == 8) ?
                                        (~(uint64_t)0) :
                                        ((uint64_t)1 << (8 * sizeof(seq_t))) - 1;

    /* Initialise cache storage. */
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
    }

    /* Generate terms until we have at least n. */
    while (cache_sz < (size_t)n) {
        last_k++;
        uint64_t cand64 = 10ULL * last_k + 7ULL;

        /* If candidate exceeds what seq_t can hold, stop further search. */
        if (cand64 > MAX_SEQ_VAL) break;

        seq_t cand = (seq_t)cand64;

        if (!isPrimeGeneric(cand))
            continue;

        seq_t a1 = cand - 6;   /* 10k+1 */
        seq_t a3 = cand - 4;   /* 10k+3 */
        seq_t a9 = cand + 2;   /* 10k+9 */

        /* Each of a1,a3,a9 must be >1 and non‑prime. */
        if (a1 < 2 || isPrimeGeneric(a1)) continue;
        if (a3 < 2 || isPrimeGeneric(a3)) continue;
        if (a9 < 2 || isPrimeGeneric(a9)) continue;

        /* Store the accepted candidate. */
        if (cache_sz >= cache_cap) {
            cache_cap *= 2;
            cache = mem_resize(seq_t, cache, cache_cap);
        }
        cache[cache_sz++] = cand;
    }

    /* If cache_sz < n because we ran out of representable values,
     * return the last stored term (or zero). */
    if (cache_sz < (size_t)n) {
        return (cache_sz > 0) ? cache[cache_sz - 1] : 0;
    }

    /* a(1) is stored at index 0. */
    return cache[n - 1];
}
