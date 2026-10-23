/*
 * A015349  Gaussian binomial coefficient [ n,7 ] for q = -9.
 * https://oeis.org/A015349
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 7, i32 n <= 8, i64 n <= 9, i128 n <= 12,
 *            big any n
 * Helpers:   seq_mul, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula: a(n) = Gaussian binomial coefficient [n,7] for q = -9 */
/* Implemented by dynamic forward iteration using the recurrence
   [n,k] = [n-1,k] + q^{n-k} * [n-1,k-1]  
   with [0,0] = 1 and [n,0] = 1.  The sequence is memoised. */

#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_mul.h"
#include "../utils/seq_realloc.h"

static const seq_t Q = -9;          /* q value */

static seq_t *pow_q = NULL;         /* integer array of powers of Q,
                                      pow_q[e] == Q^e  */
static size_t pow_q_len = 0;        /* current allocated length */

static seq_t *cache = NULL;         /* cached a(n) values for n >= 7 */
static size_t cache_len = 0;        /* current allocated length of cache */

static seq_t cur[8];                /* values of [current_n, k] for k=0..7 */
static idx_t cur_n = 0;             /* current n for which cur holds values */

static void
ensure_powq(size_t needed)
{
    if (needed < pow_q_len)
        return;
    size_t old = pow_q_len;
    size_t new_len = needed + 1;
    pow_q = seq_realloc(pow_q, old, new_len);
    if (old == 0) {
        pow_q[0] = (seq_t)1;
        old = 1;
    }
    for (size_t i = old; i <= needed; ++i)
        pow_q[i] = seq_mul(pow_q[i - 1], Q);
    pow_q_len = new_len;
}

static void
ensure_cache(size_t needed)
{
    if (needed < cache_len)
        return;
    size_t old = cache_len;
    size_t new_len = needed + 1;
    cache = seq_realloc(cache, old, new_len);
    for (size_t i = old; i <= needed; ++i)
        cache[i] = (seq_t)0;
    cache_len = new_len;
}

/* Main sequence function */
seq_t
a(idx_t n)
{
    if (n < 7)
        return (seq_t)0;            /* undefined for n < 7, as requested */

    /* If we have already computed n, return cached value */
    if (cache_len > (size_t)n && cache[n] != (seq_t)0)
        return cache[n];

    /* Initialise to n = 0 if this is the first call */
    if (cur_n == 0) {
        for (int i = 0; i < 8; ++i)
            cur[i] = (seq_t)0;
        cur[0] = (seq_t)1;          /* [0,0] = 1 */
        cur_n = 0;
    }

    /* Iterate forward until we reach the requested n */
    while (cur_n < n) {
        idx_t next_n = cur_n + 1;
        ensure_powq((size_t)(next_n - 1));   /* need powers up to next_n-1 */

        seq_t nxt[8];
        nxt[0] = (seq_t)1;
        for (size_t i = 1; i < 8; ++i) {
            if (i > (size_t)next_n)
                nxt[i] = (seq_t)0;
            else {
                seq_t term = seq_mul(pow_q[(size_t)(next_n - i)], cur[i - 1]);
                nxt[i] = cur[i] + term;
            }
        }

        /* copy next row into cur */
        for (size_t i = 0; i < 8; ++i)
            cur[i] = nxt[i];

        cur_n = next_n;

        if (next_n >= 7) {            /* store a(next_n) in cache */
            ensure_cache((size_t)next_n);
            cache[next_n] = cur[7];
        }
    }

    /* After the loop cur_n == n, so cur[7] holds the answer */
    return cur[7];
}
