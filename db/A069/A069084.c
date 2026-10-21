/*
 * A069084  Numbers n such that sigma(n)/tau(n) has denominator 5.
 * https://oeis.org/A069084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n‑th integer m whose mean divisor σ(m)/τ(m) (σ = sum
 *            of divisors, τ = number of divisors) reduces to a fraction with
 *            denominator 5. The sequence is obtained by scanning consecutive
 *            integers, computing σ and τ via prime factorisation, reducing
 *            the fraction, and keeping those with denomi...
 * Range:     u8 n <= 8, u16 n <= 1734, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_denom_five
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Standard headers – available both in C11 and C++17. */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/has_denom_five.h"

/*  Cache for the sequence so that repeated calls are efficient.  */
static seq_t *cache = NULL;       /* array of a(1), a(2), … */
static size_t cache_len  = 0;     /* number of stored terms */
static size_t cache_cap  = 0;     /* allocated capacity */
static uint64_t next_candidate = 1;  /* next natural number to test */

/*  Ensure cache is large enough to hold 'need' terms.  */
static void ensure_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *new_arr = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_arr;
    cache_cap = new_cap;
}

/*  Main sequence generator.  */
seq_t a(idx_t n)
{
    /*  The sequence is 1‑based.  */
    if (n == 0) return 0;               /* undefined, but harmless */

    while (cache_len < (size_t)n) {
        ensure_cache(cache_len + 1);
        if (has_denom_five(next_candidate)) {
            cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
