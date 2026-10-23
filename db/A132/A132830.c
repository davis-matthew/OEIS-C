/*
 * A132830  Numbers of the form 3^n+2 which are not primes.
 * https://oeis.org/A132830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 12, u64 n <= 29, u128 n <= 67,
 *            big any n
 * Helpers:   is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the n-th integer of the form 3^k + 2 that is not prime.
 * The implementation evaluates 3^k + 2 for increasing k starting with k = 1,
 * checks whether the result is composite (using a fast deterministic test
 * for 64‑bit values and the supplied is_prime_big routine for larger ones),
 * stores composite occurrences, and returns the n‑th stored value.
 * Memoisation guarantees O(1) average time for repeated calls.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <limits.h>                      /* UINT64_MAX */

#include "../utils/is_prime_big.h"      /* bool is_prime_big(seq_t n) */
#include "../utils/seq_realloc.h"       /* seq_t* seq_realloc(seq_t*,size_t,size_t) */

/* ----------  Helper: deterministic Miller‑Rabin for 64‑bit integers ------------- */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)                     return false;
    /* small primes */
    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        if (n == small[i]) return true;
        if (n % small[i] == 0) return false;
    }

    /* decomposition of n-1 = d * 2^s with d odd */
    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    /* bases sufficient for 64‑bit integers */
    static const uint64_t bases[] = {2,3,5,7,11,13,17};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i];
        if (a % n == 0) continue;

        /* compute a^d mod n */
        __uint128_t x = 1, p = a;
        uint64_t e = d;
        while (e) {
            if (e & 1) x = (x * p) % n;
            p = (p * p) % n;
            e >>= 1;
        }
        uint64_t xi = (uint64_t)x;
        if (xi == 1 || xi == n - 1) continue;

        bool continue_outer = false;
        for (unsigned r = 1; r < s; ++r) {
            x = (__uint128_t)xi * xi % n;
            xi = (uint64_t)x;
            if (xi == n - 1) { continue_outer = true; break; }
        }
        if (continue_outer) continue;
        return false;               /* composite */
    }
    return true;                    /* prime */
}

/* ----------  General composite test that uses the 64‑bit routine when possible  ---------- */
static bool is_composite_generic(seq_t n)
{
    /* if n fits into 64 bits, use the fast test */
    if (n <= (seq_t)UINT64_MAX) {
        uint64_t x = (uint64_t)n;
        return !is_prime_uint64(x);
    }
    /* otherwise use the provided generic routine */
    return !is_prime_big(n);
}

/* ----------  Main sequence function  --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* below offset – return 0 */

    /* static state */
    static idx_t next_exp  = 1;      /* exponent to test next (1‑based) */
    static seq_t *cache    = NULL;   /* memoised composite values */
    static size_t cap      = 0;
    static size_t cnt      = 0;
    static bool  init      = false;
    static seq_t current;           /* 3^next_exp */

    if (!init) {
        current = 3;                /* 3^1 */
        init    = true;
    }

    /* compile‑time indication whether seq_t is 64‑bit or larger */
    static const bool LARGE = (sizeof(seq_t) > sizeof(uint64_t));

    /* generate values until we have at least n composites */
    while (cnt < n) {
        /* stop if the next value would overflow the 64‑bit type */
        if (!LARGE && current > (seq_t)UINT64_MAX - 2) break;

        seq_t val = current + 2;    /* candidate 3^k + 2 */

        if (is_composite_generic(val)) {   /* store composite only */
            if (cnt == cap) {
                size_t new_cap = cap == 0 ? 8 : cap << 1;
                seq_t *tmp = seq_realloc(cache, cap, new_cap);
                cache = tmp;
                cap   = new_cap;
            }
            cache[cnt++] = val;
        }

        /* prepare next power of 3 */
        if (!LARGE) {
            unsigned __int128 prod = (unsigned __int128)current * 3;
            if (prod > (unsigned __int128)UINT64_MAX) break;   /* will overflow */
            current = (seq_t)prod;
        } else {
            current = current * 3;
        }
        ++next_exp;
    }

    /* if we run out of space (overflow reached), return 0 */
    if (cnt < n) return 0;

    return cache[n - 1];
}
