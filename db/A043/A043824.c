/*
 * A043824  Numbers k such that number of runs in the base 3 representation of
 *          k is congruent to 9 mod 10.
 * https://oeis.org/A043824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043824:
   Numbers k such that the number of runs in the base‑3 representation
   of k is congruent to 9 modulo 10.
   The function evaluates terms by brute‑force search. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* helper: grow a dynamic array of seq_t */
static void grow_cache(seq_t **p, size_t *cap, size_t need)
{
    size_t new_cap = (*cap == 0) ? 16 : (*cap * 2);
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(*p, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* out of memory */
    *p   = tmp;
    *cap = new_cap;
}

/* helper: test whether a given k satisfies the property */
static bool matches(idx_t k)
{
    /* digit array large enough for 3^32 > 2^64 */
    uint8_t digits[32];
    size_t len = 0;
    if (k == 0) { digits[0] = 0; len = 1; }
    while (k > 0) {
        digits[len++] = (uint8_t)(k % 3);
        k /= 3;
    }
    size_t runs = 1;
    for (size_t i = 1; i < len; ++i) {
        if (digits[i] != digits[i - 1]) ++runs;
    }
    return (runs % 10) == 9;
}

/* main sequence function */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t sz    = 0;
    static idx_t last_k = 0;   /* last tested k */

    while (sz < n) {
        ++last_k;                            /* next candidate */
        if (matches(last_k)) {
            if (sz == cap) grow_cache(&cache, &cap, sz + 1);
            cache[sz++] = (seq_t)last_k;
        }
    }
    return cache[n - 1];
}
