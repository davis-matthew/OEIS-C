/*
 * A062679  Numbers such that every divisor (except 1, but including the
 *          number itself) contains the digit 9.
 * https://oeis.org/A062679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) is the nth integer whose every divisor (except 1) contains
 *            the digit 9
 * Range:     u8 n <= 16, u16 n <= 3140, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _factor_u64, check_all_divs, contains9_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation searches successive integers that contain the digit 9
   and validates that all their divisors contain 9. A cache is used to
   avoid recomputation for arbitrary call order. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/_factor_u64.h"
#include "../utils/check_all_divs.h"
#include "../utils/contains9_uint64.h"

/* Check if a candidate satisfies the sequence property. */
static bool is_good_candidate(uint64_t cand)
{
    if (!contains9_uint64(cand))
        return false;                       /* the number itself must contain 9 */

    uint64_t pr[64], exp[64];
    size_t cnt;
    _factor_u64(cand, pr, exp, &cnt);       /* factorisation */

    /* Every prime factor must contain the digit 9. */
    for (size_t i = 0; i < cnt; ++i)
        if (!contains9_uint64(pr[i]))
            return false;

    /* All composite divisors must also contain 9. */
    return check_all_divs(pr, exp, 0, 1, (int)cnt);
}

/* Cache for memoised terms. */
static seq_t *a_cache      = NULL;   /* array of terms a(1), a(2), ... */
static size_t cache_cap    = 0;       /* allocated capacity */
static size_t cache_len    = 0;       /* number of terms stored so far */
static uint64_t last_cand  = 19;      /* next integer to test */

/* Ensure the cache can hold at least 'need' terms. */
static void ensure_cache(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *new_p = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    a_cache = new_p;
    cache_cap = new_cap;
}

/* Main sequence routine. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0; /* undefined for indices below offset */

    /* Fast return if already cached. */
    if ((size_t)n <= cache_len)
        return a_cache[n - 1];

    /* Extend cache to at least n terms. */
    ensure_cache((size_t)n + 1); /* some slack */

    /* Generate new terms until we reach n. */
    while (cache_len < (size_t)n) {
        while (true) {
            if (is_good_candidate(last_cand)) {
                a_cache[cache_len++] = (seq_t)last_cand;
                ++last_cand;
                break;
            }
            ++last_cand;
        }
    }
    return a_cache[n - 1];
}
