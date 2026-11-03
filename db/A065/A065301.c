/*
 * A065301  Numbers k such that both k and the sum of its divisors are
 *          squarefree numbers.
 * https://oeis.org/A065301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 2450, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9763 (2026-11-03)
 * Helpers:   is_squarefree, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: generate numbers k such that k and sigma(k) are squarefree
 * by scanning integers and storing the first n valid terms. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_squarefree.h"
#include "../utils/mem.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    static seq_t      *cache = NULL;  /* cached terms */
    static size_t      cache_len = 0; /* number of cached terms */
    static size_t      cache_cap = 0; /* allocated capacity */
    static idx_t       next_i  = 1;   /* next integer to test */

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        idx_t i = next_i++;                  /* current integer to test   */
        if (is_squarefree( (seq_t)i )) {
            seq_t sigma = sigma_of(i);       /* sum of divisors of i      */
            if (is_squarefree(sigma)) {
                grow_seq( &cache, &cache_cap, cache_len + 1 );
                cache[cache_len++] = (seq_t)i;
            }
        }
    }
    return cache[n-1];
}
