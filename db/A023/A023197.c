/*
 * A023197  Numbers k such that sigma(k) >= 3*k.
 * https://oeis.org/A023197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A023197 – Numbers k such that sigma(k) >= 3*k
 * Range:     u8 none, u16 n <= 626, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *sigma_array = NULL;   /* divisor sums */
static idx_t sigma_limit = 0;       /* current max n covered by sigma_array */

static seq_t *seq_cache = NULL;    /* cached terms of A023197 */
static size_t seq_len = 0;          /* number of cached terms */
static size_t seq_cap = 0;          /* capacity of seq_cache */

/* append a new term to seq_cache, growing the array if needed */
static void append_term(seq_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap < 16 ? 16 : seq_cap * 2;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);            /* fatal if allocation fails */
        seq_cache = tmp;
        seq_cap = new_cap;
    }
    seq_cache[seq_len++] = val;
}

/* rebuild sigma_array and seq_cache up to new_limit */
static void rebuild(idx_t new_limit)
{
    /* allocate new sigma array */
    seq_t *new_sigma = (seq_t *)calloc((size_t)new_limit + 1, sizeof(seq_t));
    if (!new_sigma) exit(1);

    /* divisor-sum sieve */
    for (idx_t i = 1; i <= new_limit; ++i) {
        seq_t inc = (seq_t)i;
        for (idx_t j = i; j <= new_limit; j += i) {
            new_sigma[j] += inc;
        }
    }

    /* build new seq_cache */
    seq_len = 0;
    for (idx_t k = 1; k <= new_limit; ++k) {
        seq_t s = new_sigma[k];
        if (s >= (seq_t)3 * (seq_t)k) {
            append_term(k);
        }
    }

    /* replace old array */
    free(sigma_array);
    sigma_array = new_sigma;
    sigma_limit = new_limit;
}

/* ensure we have at least n terms cached */
static void ensure(idx_t n)
{
    if (seq_len >= n) return;

    /* start with a reasonable guess */
    idx_t limit = sigma_limit ? sigma_limit : 1000;
    while (seq_len < n) {
        if (limit == 0) limit = 1;
        rebuild(limit);
        if (seq_len >= n) break;
        /* double limit if insufficient */
        limit *= 2;
    }
}

/* the required function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but safe */
    ensure(n);
    return seq_cache[n - 1];
}
