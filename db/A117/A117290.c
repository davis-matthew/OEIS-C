/*
 * A117290  Numbers k for which the cototient k - phi(k) is a Fibonacci
 *          number.
 * https://oeis.org/A117290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 6631, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi, is_fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n): numbers k such that the cototient k - phi(k) is a Fibonacci number.
 * We generate terms by scanning k = 1,2,… and testing the condition.
 */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "../utils/calc_phi.h"
#include "../utils/is_fib.h"

/* --- cache of produced terms ----------------------------------------------------- */
static seq_t *cache   = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* --- next candidate to test ----------------------------------------------------- */
static idx_t next_candidate = 1;

/* --- enlarge the cache array if necessary --------------------------------------- */
static void ensure_cache_capacity(size_t need)
{
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }
    while (cache_len + need > cache_cap) {
        cache_cap *= 2;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
}

/* --- generate terms until we have at least n terms ------------------------------- */
static void generate_up_to(size_t n)
{
    while (cache_len < n) {
        seq_t k      = (seq_t)next_candidate;
        seq_t phi_k  = calc_phi(k);
        seq_t diff   = k - phi_k;
        if (is_fib(diff)) {
            ensure_cache_capacity(1);
            cache[cache_len++] = k;
        }
        ++next_candidate;
    }
}

/* --- OEIS entry --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)      /* out of range, return 0 for safety */
        return (seq_t)0;

    size_t needed = (size_t)n;
    generate_up_to(needed);
    return cache[needed - 1];   /* a(1) -> cache[0] */
}
