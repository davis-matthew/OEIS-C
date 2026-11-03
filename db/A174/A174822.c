/*
 * A174822  Primes of form n^4 - n^2 - 1.
 * https://oeis.org/A174822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Validated: partial, pass, 29 terms through n = 29 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A174822 – Primes of the form n^4 – n^2 – 1  */
/*  The function returns the nth such prime (1‑based index).  */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
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

#ifdef __cplusplus
# include <vector>
#endif

seq_t a(idx_t n) {
    /* Each sequence element is defined as:  p(k) = k^4 – k^2 – 1  for k ≥ 2
       where p(k) is prime.  This routine builds the list of these primes
       lazily and caches them for future calls.                                   */

#ifdef __cplusplus
    /* C++ implementation using std::vector – works when seq_t is a
       user‑defined big integer class.                                            */
    static std::vector<seq_t> cache;
    static idx_t next_k = 2;

    while (cache.size() < n) {
        seq_t k   = (seq_t)next_k;
        seq_t k2  = k * k;
        seq_t k4  = k2 * k2;
        seq_t val = k4 - k2 - (seq_t)1;
        if (isPrimeGeneric(val))
            cache.push_back(val);
        ++next_k;
    }
    return cache[n - 1];
#else
    /* C implementation – seq_t is a plain integer type, so realloc is safe. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t next_k = 2;
    static size_t len   = 0;

    while (len < n) {
        seq_t k   = (seq_t)next_k;
        seq_t k2  = k * k;
        seq_t k4  = k2 * k2;
        seq_t val = k4 - k2 - (seq_t)1;
        if (isPrimeGeneric(val)) {
            if (len == cap) {
                size_t newcap = cap ? cap * 2 : 8;
                seq_t *ptr    = realloc(cache, newcap * sizeof(seq_t));
                if (!ptr) return 0;   /* allocation failed */
                cache = ptr;
                cap   = newcap;
            }
            cache[len++] = val;
        }
        ++next_k;
    }
    return cache[n - 1];
#endif
}
