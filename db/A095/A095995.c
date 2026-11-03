/*
 * A095995  Primes of the form 100n - 1.
 * https://oeis.org/A095995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 157, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 55 terms through n = 841 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computation of OEIS A095995: primes of the form 100 m − 1.
 * The sequence is enumerated by increasing m starting at 2.
 * a(1) = 199 = 100 × 2 − 1, a(2) = 499 = 100 × 5 − 1, …
 * The function returns the n‑th such prime (n ≥ 1); for n = 0 it returns 0.
 * Internally, it caches the primes found so far and continues the search
 * on subsequent calls.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/*
 * seq_t and idx_t are supplied by the build system.
 * seq_t is an unsigned integer type (possibly arbitrary precision).
 * idx_t is uint64_t.
 */

seq_t a(idx_t n)
{
    /* Undefined for offset zero; return zero for safety. */
    if (n == 0)
        return 0;

    /* Static cache of found primes. */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;
    static size_t  len   = 0;
    static idx_t   m     = 2;      /* last m examined */

    /* Extend cache until we have at least n primes. */
    while (len < n) {
        /* Compute candidate 100*m − 1 using 64‑bit arithmetic to avoid
           overflow in the intermediate product. */
        uint64_t cand64 = (uint64_t)m * 100ULL - 1ULL;
        seq_t   cand   = (seq_t)cand64;

        if (is_prime_big(cand)) {
            /* Grow the cache if necessary. */
            if (len == cap) {
                size_t new_cap = (cap == 0) ? 8 : cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (tmp == NULL) {
                    /* Allocation failed; abort with a defined behaviour. */
                    /* In a real implementation we might handle this differently,
                       but for the purposes of the sequence generator we can
                       simply exit. */
                    abort();
                }
                cache = tmp;
                cap   = new_cap;
            }
            cache[len++] = cand;
        }
        ++m;
    }

    return cache[(size_t)(n - 1)];
}
