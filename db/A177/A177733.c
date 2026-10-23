/*
 * A177733  Integers that can be expressed as the sum of two or more positive
 *          consecutive numbers (the largest being even) AND also as the sum
 *          of two or more positive consecutive numbers (the largest being
 *          odd).
 * https://oeis.org/A177733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Brute‑force search for the nth integer that can be expressed as
   a sum of two or more consecutive positive integers in two
   different ways, one with an even largest term and one with an
   odd largest term. */

/* Requires seq_t and idx_t already defined. */
#include <stdlib.h>
#include <stdbool.h>

#ifdef __cplusplus
#include <vector>
#endif

/* Helper: test if a value S can be expressed as two required sums. */
static bool has_two_types(seq_t S)
{
    seq_t twoS = S + S;                /* 2*S */
    bool odd_L  = false;
    bool even_L = false;

    for (idx_t k = 2; (seq_t)k * k <= twoS; ++k) {
        if (twoS % (seq_t)k != 0) continue;
        seq_t m = twoS / (seq_t)k;          /* m = 2*a + k - 1 */
        if (m <= (seq_t)k) continue;        /* need a >= 1 */
        seq_t diff = m - (seq_t)k + 1;      /* 2*a  */
        if (diff % 2u != 0u) continue;
        seq_t a = diff / 2u;
        seq_t L = a + (seq_t)k - 1u;        /* largest term */
        if ((L & 1u) == 0u) even_L = true;
        else                 odd_L  = true;
        if (odd_L && even_L) return true;
    }
    return odd_L && even_L;
}

#ifdef __cplusplus
/* C++ implementation uses a std::vector cache. */
static std::vector<seq_t> seq_cache;
static seq_t seq_last_checked = 0;

/* Return the nth term (n>=1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined, safe value */

    /* Expand the cache until it contains at least n elements. */
    while (seq_cache.size() < n) {
        seq_t s = seq_last_checked + 1;
        seq_last_checked = s;
        if (has_two_types(s))
            seq_cache.push_back(s);
    }
    return seq_cache[n - 1];
}

#else
/* C implementation uses a dynamic array. */
static seq_t *seq_cache = NULL;          /* array of found terms        */
static idx_t   seq_cache_len = 0;        /* number of elements stored  */
static seq_t seq_last_checked = 0;

/* Return the nth term (n>=1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined, safe value */

    /* Expand the array until it contains at least n elements. */
    while (seq_cache_len < n) {
        seq_t s = seq_last_checked + 1;
        seq_last_checked = s;
        if (has_two_types(s)) {
            idx_t new_len = seq_cache_len + 1;
            seq_t *tmp = (seq_t *)realloc(seq_cache,
                                            new_len * sizeof(seq_t));
            if (!tmp) abort();              /* allocation failure */
            seq_cache = tmp;
            seq_cache[seq_cache_len++] = s;
        }
    }
    return seq_cache[n - 1];
}

#endif
