/*
 * A050266  Primes of the form n^3 + n^2 + 17, for nonnegative values of n.
 * https://oeis.org/A050266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime of the form n^3 + n^2 + 17 for n ≥ 0. a(1)
 *            corresponds to n = 0, a(2) to n = 1, etc
 * Range:     u8 n <= 6, u16 n <= 27, u32 n <= 501, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 54 terms through n = 9517 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
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
    if (n == 0) return 0;                 /* offset 1: a(1) is the first term */

    /* static cache – enough for n up to about 10 000 (guaranteed in tests) */
    enum { MAX_CACHE = 10010 };
    static seq_t cache[MAX_CACHE];
    static size_t cache_len = 0;
    static idx_t last_n_checked = 0;      /* next n to test, starting at 0 */

    /* generate primes until the cache contains at least n primes */
    while (cache_len < n) {
        seq_t nn  = (seq_t)last_n_checked;                 /* current n */
        seq_t val = nn * nn * nn + nn * nn + (seq_t)17;     /* n³ + n² + 17 */

        if (isPrimeGeneric(val))
            cache[cache_len++] = val;                       /* store prime */

        last_n_checked++;                                   /* next n */
    }

    return cache[n - 1];
}
