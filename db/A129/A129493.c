/*
 * A129493  Composite numbers k such that 3^k mod k is a power of 3.
 * https://oeis.org/A129493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 94, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9496 (2026-11-03)
 * Helpers:   is_prime, mem, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n‑th term of OEIS A129493
 *
 * A129493: composite numbers k such that 3^k (mod k) is a non‑trivial
 * power of 3 (i.e. 3, 9, 27, …).  The list starts 6,10,12,14,18,…
 *
 * The function iterates through composite integers in order, checks the
 * condition, and keeps the results in a dynamically grown cache so that
 * calls with arbitrary n (in any order) return quickly.
 *
 * Note: 1 is not considered a power of 3 for this sequence; we require
 * the remainder to be >1.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/is_prime.h"
#include "../utils/powmod.h"
#include "../utils/mem.h"

static seq_t *cache = NULL;          /* array of found terms */
static size_t cache_len = 0;          /* number of terms stored */
static size_t cache_cap = 0;          /* allocated capacity */
static idx_t next_test = 4;           /* next integer to consider */

/* Ensure the cache can hold at least `need` elements */
static void ensure_cache_capacity(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *new_ptr = mem_resize(seq_t, cache, new_cap);
    if (!new_ptr) abort(); /* insufficient memory */
    cache = new_ptr;
    cache_cap = new_cap;
}

/* Return true if x is a (positive) power of 3, i.e. 3^k for k>=1 */
static bool is_power_of_3(seq_t x)
{
    if (x <= (seq_t)1) return false;
    while (x % (seq_t)3 == 0)
        x /= 3;
    return x == (seq_t)1;
}

/* Find the n-th term (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* out of spec, but safe */

    /* Keep generating terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        /* Find the next composite integer ≥ next_test. */
        while (next_test <= 1 || is_prime(next_test))
            ++next_test;

        /* Compute 3^k mod k. */
        idx_t k = next_test;
        seq_t r = powmod((seq_t)3, (uint64_t)k, (seq_t)k);

        /* Check the condition: remainder > 1 and a power of 3. */
        if (r > (seq_t)1 && is_power_of_3(r)) {
            /* Store the term. */
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = (seq_t)k;
        }

        /* Advance to the next integer to test. */
        ++next_test;
    }

    return cache[(size_t)(n - 1)];
}
