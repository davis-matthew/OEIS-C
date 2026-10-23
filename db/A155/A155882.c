/*
 * A155882  Smallest positive prime number such that a(n)-2n is also prime,
 *          a(n) < a(n+1), and the differences a(n)-2n must increase with n.
 * https://oeis.org/A155882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   to_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A155882:  Smallest positive prime p such that
 *  (1) p is prime,
 *  (2) p - 2*n is prime,
 *  (3) p - 2*n strictly increases with n,
 *  (4) a(n) < a(n+1).
 * The function memoises computed terms so that any call order is supported.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/to_uint64.h"

/* Simple deterministic primality test for 64‑bit unsigned integers. */
static bool is_prime64(uint64_t n)
{
    if (n < 2)                return false;
    if (n % 2 == 0)           return n == 2;
    if (n % 3 == 0)           return n == 3;
    for (uint64_t i = 5; i <= n / i; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

/* Convert a seq_t value to a 64‑bit unsigned integer for primality testing.
 * The build system guarantees that the values needed for the sequence
 * fit into 64 bits, so this cast is safe for all valid inputs. */

/* ----------------------------------------------------------------------
 * Memoised sequence array.
 * a_cache[i]  holds a(i+1) for i >= 0.
 * cache_len   is the number of values currently stored.
 * cache_cap   allocated capacity of a_cache.
 * ---------------------------------------------------------------------- */
static seq_t* a_cache   = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Ensure the cache has capacity for at least 'need' elements. */
static void ensure_cache_capacity(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    while (new_cap < need) new_cap <<= 1;
    a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* ----------------------------------------------------------------------
 * Main function required by the harness.
 * ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but return 0 for safety */

    /* Compute needed terms up to n */
    if (cache_len < (size_t)n) {
        seq_t diff_last = 0;      /* difference of the previous term */

        for (size_t i = cache_len; i < (size_t)n; ++i) {
            /* Candidate prime starts just > previous term, or 2 for the first term. */
            seq_t cand = (i == 0) ? 2 : a_cache[i - 1] + 1;

            /* Search forward until a valid candidate is found. */
            for (;;) {
                if (!is_prime64(to_uint64(cand))) { cand++; continue; }

                seq_t d = cand - (seq_t)(i + 1) * 2;  /* d = p - 2*n */
                if (is_prime64(to_uint64(d)) && d > diff_last) {
                    /* Accept this term. */
                    ensure_cache_capacity(i + 1);
                    a_cache[i] = cand;
                    diff_last = d;
                    break;
                }
                cand++;
            }
        }
    }

    /* Return the requested term (0‑based cache). */
    return a_cache[n - 1];
}
