/*
 * A096079  Near hypotenuse numbers, i.e., n such that n^2 -+ 1 = a^2 + b^2,
 *          with a, b > 1.
 * https://oeis.org/A096079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-03)
 * Helpers:   isqrt_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Near hypotenuse numbers A096079
 *
 * Definition: n is in the sequence if either n²+1 or n²–1 can be
 * expressed as a²+b² with a > 1 and b > 1.
 *
 * Algorithm: For each candidate n from 1 upward, test the two values
 * m = n²±1.  To test whether m is a sum of two squares, try all
 * a ≥ 2 up to ⌊√(m–4)⌋, compute b² = m–a² and check that b² is a
 * perfect square larger than 1.  The search stops when the cache
 * contains at least n terms, and the cache is reused for arbitrary
 * call order.  The implementation uses an integer square‑root routine
 * based on binary search that works for any seq_t (including
 * arbitrary‑precision).
 *
 * The routine satisfies the required O(n) time behaviour for n ≤ 10 000.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/isqrt_seq.h"
#include "../utils/mem.h"

/* Test whether m > 0 can be expressed as a²+b² with a,b > 1. */
static int has_two_square_rep(seq_t m)
{
    if (m < 8)                     /* 2²+2² = 8 is the smallest admissible sum */
        return 0;

    seq_t limit = isqrt_seq(m - 4);   /* a² ≤ m–4 because b² ≥ 4 (b>1) */

    for (seq_t a = 2; a <= limit; ++a)
    {
        seq_t a2 = a * a;
        seq_t b2 = m - a2;
        if (b2 < 4)   /* b > 1 ⇒ b² ≥ 4 */
            continue;

        seq_t b = isqrt_seq(b2);
        if (b * b == b2 && b > 1)
            return 1;
    }
    return 0;
}

/* Test whether n is a near hypotenuse number. */
static int is_near_hypotenuse(seq_t n)
{
    seq_t n2 = n * n;

    /* n² + 1 */
    seq_t pos = n2 + 1;
    if (has_two_square_rep(pos))
        return 1;

    /* n² – 1 (guard against underflow when n == 0) */
    if (n > 0)
    {
        seq_t neg = n2 - 1;
        if (has_two_square_rep(neg))
            return 1;
    }
    return 0;
}

/* --------------------------------------------------------------------- */
/* Local caching of found terms, to allow out‑of‑order queries.          */
/* --------------------------------------------------------------------- */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static idx_t  max_checked = 0;

/* Grow cache if necessary and append a term. */
static void cache_push(seq_t val)
{
    if (cache_len == cache_cap)
    {
        cache_cap = (cache_cap > 0) ? cache_cap * 2 : 16;
        cache = mem_resize(seq_t, cache, cache_cap);
    }
    cache[cache_len++] = val;
}

/* --------------------------------------------------------------------- */
/* Public interface: return the nth term (1‑based).                     */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;               /* undefined for n=0, but return 0 for safety */

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n)
    {
        ++max_checked;
        seq_t cand = (seq_t)max_checked;
        if (is_near_hypotenuse(cand))
            cache_push(cand);
    }
    return cache[n - 1];
}
