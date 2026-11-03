/*
 * A043790  Number of runs in the base 3 representation of n is congruent to 5
 *          mod 6.
 * https://oeis.org/A043790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th integer m such that the number of runs in the base‑3
 *            representation of m is congruent to 5 mod 6
 * Range:     u8 n <= 34, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Helper: test whether number x has run count ≡ 5 (mod 6) in base 3 */
static int runs_mod6_eq5(seq_t x)
{
    if (x == 0) return 0;          /* 0 has 0 runs, not 5 mod 6 */
    seq_t last_digit = x % 3;
    x /= 3;
    seq_t runs = 1;                /* counted the MSB digit above */
    while (x != 0) {
        seq_t d = x % 3;
        if (d != last_digit) {
            ++runs;
            last_digit = d;
        }
        x /= 3;
    }
    return (runs % 6) == 5;
}

static seq_t *cache = NULL;        /* cache of computed sequence elements */
static size_t cache_cap = 0;       /* capacity of cache array */
static size_t cache_len = 0;       /* number of elements stored */
static seq_t last_checked = 0;     /* largest number tested so far */

/* Core function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined for n < 1, return 0 */
    /* Ensure cache holds at least n elements */
    while (cache_len < (size_t)n) {
        /* Expand cache if needed */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            if (new_cap < (size_t)n) new_cap = n;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp) abort();    /* memory allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }
        /* Test successive integers starting after last_checked */
        seq_t candidate = last_checked + 1;
        while (!runs_mod6_eq5(candidate)) {
            ++candidate;
        }
        /* Store the candidate */
        cache[cache_len++] = candidate;
        last_checked = candidate;
    }
    return cache[n - 1];
}
