/*
 * A174237  Numbers n such that 4n-1 and 4n+3 are prime.
 * https://oeis.org/A174237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is searched as the n‑th integer such that 4n-1 and 4n+3 are
 *            prime
 * Range:     u8 n <= 10, u16 n <= 618, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 71 terms through n = 978 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
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
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static idx_t next_test = 1;          /* next candidate to test */

    while (cache_size < n)
    {
        seq_t cand = (seq_t)next_test;
        seq_t p1   = (seq_t)4 * cand - 1;
        seq_t p2   = (seq_t)4 * cand + 3;

        if (isPrimeGeneric(p1) && isPrimeGeneric(p2))
        {
            if (cache_size == cache_cap)
            {
                cache_cap = cache_cap == 0 ? 8 : cache_cap * 2;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_size++] = cand;
        }

        if (next_test == 1)
            next_test = 2;        /* handle the special case n == 1 */
        else
            next_test += 3;       /* only numbers ≡ 2 (mod 3) can succeed thereafter */
    }

    return cache[n - 1];
}
