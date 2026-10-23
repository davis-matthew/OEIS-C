/*
 * A169522  Number of reduced words of length n in Coxeter group on 29
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^34 = I.
 * https://oeis.org/A169522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A169522: Number of reduced words of length n in the Coxeter group on
 * 29 generators S_i with relations (S_i)^2 = (S_i S_j)^34 = I.
 *
 * The generating function is rational, yielding a linear recurrence.
 * For 0 ≤ n ≤ 33:
 *     a(n) = 27·∑_{i=1}^{n} a(n−i) + c(n)         (c(0)=1, c(n)=2 for 1≤n≤33)
 * For n = 34:
 *     a(34) = 27·∑_{i=1}^{33} a(34−i) + 1 − 378·a(0)
 * For n ≥ 35:
 *     a(n) = 27·∑_{i=1}^{33} a(n−i) − 378·a(n−34)
 */
#include <stdlib.h>

static seq_t *seq_cache = NULL;
static idx_t cache_size = 0;
static size_t cache_cap   = 0;

static void ensure_cache(idx_t n)
{
    if (cache_size > n)
        return;

    /* enlarge storage */
    size_t want = (cache_cap == 0 ? 64 : cache_cap);
    while (want <= (size_t)n)
        want <<= 1;
    seq_t *new_cache = (seq_t *)realloc(seq_cache, want * sizeof(seq_t));
    if (!new_cache) {
        /* allocation failure: abort */
        exit(1);
    }
    seq_cache = new_cache;
    cache_cap = want;

    /* compute terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) {
            seq_cache[0] = (seq_t)1;
            continue;
        }

        if (i <= 33) {
            seq_t sum = 0;
            for (idx_t j = 1; j <= i; ++j)
                sum += seq_cache[i - j];
            seq_t c = (seq_t)2;
            seq_cache[i] = sum * 27 + c;
        }
        else if (i == 34) {
            seq_t sum33 = 0;
            for (idx_t j = 1; j <= 33; ++j)
                sum33 += seq_cache[34 - j];
            seq_cache[34] = sum33 * 27 + (seq_t)1 - (seq_t)378 * seq_cache[0];
        }
        else { /* i >= 35 */
            seq_t sum33 = 0;
            for (idx_t j = 1; j <= 33; ++j)
                sum33 += seq_cache[i - j];
            seq_cache[i] = sum33 * 27 - (seq_t)378 * seq_cache[i - 34];
        }
    }
    cache_size = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return seq_cache[n];
}
