/*
 * A213168  a(n) = n!/2 - (n-1)! - n + 2.
 * https://oeis.org/A213168
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! / 2 - (n-1)! - n + 2 (OEIS A213168)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint64_t */
#include <stdlib.h>   /* malloc, realloc, abort */

/* Cached factorials */
static seq_t *fact_cache = NULL;   /* array of factorials: fact_cache[i] = i!   */
static idx_t  fact_cap   = 0;      /* number of elements currently stored */

/* Make sure factorials up to `n` are computed and stored in fact_cache. */
static void ensure_fact(idx_t n)
{
    if (n < fact_cap)            /* already have enough factorials */
        return;

    size_t new_cap = n + 1;       /* need indices 0..n inclusive */
    seq_t *new_cache = (seq_t *)realloc(fact_cache, new_cap * sizeof(seq_t));
    if (!new_cache)                /* out‑of‑memory; terminate */
        abort();

    /* Initialize f[0] if it does not exist yet */
    if (fact_cap == 0) {
        new_cache[0] = (seq_t)1;
        fact_cap = 1;
    }

    /* Compute missing factorial values */
    for (idx_t i = fact_cap; i <= n; ++i)
        new_cache[i] = new_cache[i - 1] * (seq_t)i;

    fact_cache = new_cache;
    fact_cap   = new_cap;
}

/* OEIS A213168 */
seq_t a(idx_t n)
{
    if (n < 2)                  /* below the offset, behaviour irrelevant */
        return (seq_t)0;

    ensure_fact(n);             /* guarantees fact_cache[n] and fact_cache[n-1] exist */

    /* a(n) = ((fact(n‑1) * (n‑2)) / 2) + 2 – n   (avoids underflow) */
    seq_t fact_prev = fact_cache[n - 1];
    seq_t term1     = fact_prev * (seq_t)(n - 2);
    seq_t half      = term1 / (seq_t)2;
    seq_t result    = half + (seq_t)2;
    result -= (seq_t)n;
    return result;
}
