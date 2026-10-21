/*
 * A065008  Integers for which the periodic part of the continued fraction for
 *          the square root of n begins with 5.
 * https://oeis.org/A065008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the nth integer N whose continued‑fraction expansion for
 *            √N has periodic part beginning with 5. The algorithm iterates
 *            over a0 = ⌊√N⌋; for each a0 it generates all d ∈ [⌊a0/3⌋+1 ,
 *            ⌊2a0/5⌋] and returns a0² + d. This yields the sequence in
 *            ascending order
 * Range:     u8 n <= 7, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;      /* cache of computed terms      */
static size_t cache_cap = 0;     /* allocated capacity           */
static size_t cache_len = 0;     /* number of stored terms       */
static idx_t max_a0 = 0;         /* largest a0 processed so far  */

/* Append a value to the cache, growing the array if necessary. */
static void add_value(seq_t v)
{
    if (cache_len >= cache_cap) {
        size_t newcap = cache_cap ? cache_cap * 2 : 128;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = newcap;
    }
    cache[cache_len++] = v;
}

/* Return the n‑th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined but harmless */

    while (cache_len < n) {
        idx_t a0 = max_a0 + 1;
        idx_t L = a0 / 3 + 1;          /* floor(a0/3)+1  */
        idx_t U = (2 * a0) / 5;        /* floor(2*a0/5)  */

        if (U >= L) {
            seq_t a0s = (seq_t)a0;     /* convert a0 to seq_t */
            for (idx_t r = L; r <= U; ++r) {
                seq_t v = a0s * a0s + (seq_t)r;
                add_value(v);
            }
        }
        max_a0 = a0;
    }
    return cache[n - 1];
}
