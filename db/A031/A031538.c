/*
 * A031538  Numbers k such that the continued fraction for sqrt(k) has even
 *          period and if the last term of the periodic part is deleted the
 *          central term is 40.
 * https://oeis.org/A031538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   floor_sqrt, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function generates the terms of OEIS sequence A031538.
 *
 * The terms are the integers k for which the simple continued fraction
 * expansion of sqrt(k) has an even period, and if the last term 2*⌊√k⌋
 * is removed from that period the remaining odd‑length sequence has its
 * central element equal to 40.  The sequence is generated on demand
 * and cached, so repeated calls with arbitrary n are fast and may be
 * arbitrary order.
 */

#include <stdbool.h>
#include <stdlib.h>

#include "../utils/floor_sqrt.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;
static idx_t next_candidate = 1;

/* Return true iff k satisfies the definition of the sequence. */
static bool passes_condition(seq_t k)
{
    seq_t a0 = floor_sqrt(k);
    if (a0 * a0 == k)          /* k is a perfect square – skip */
        return false;

    /* First pass: determine period length. */
    seq_t m = 0, d = 1, a_curr = a0;
    seq_t two_a0 = a0 * 2;

    size_t period = 0;
    while (1)
    {
        m = d * a_curr - m;
        seq_t num = k - m * m;
        d = num / d;
        seq_t a_next = (a0 + m) / d;
        period++;
        if (a_next == two_a0)               /* last term encountered */
            break;
        a_curr = a_next;
    }

    if (period % 2 != 0)   /* only even periods count */
        return false;

    /* Second pass: find the central term of the period minus the last term. */
    size_t mid_index = (period - 1) / 2;   /* zero‑based */

    m = 0; d = 1; a_curr = a0;
    size_t idx = 0;
    while (1)
    {
        m = d * a_curr - m;
        seq_t num = k - m * m;
        d = num / d;
        seq_t a_next = (a0 + m) / d;
        if (a_next == two_a0)              /* finished */
            break;

        if (idx == mid_index)
            return a_next == 40;           /* central term must be 40 */

        idx++;
        a_curr = a_next;
    }

    return false;   /* unreachable */
}

/* Append a new term to the cache, expanding the array if necessary. */
static void cache_push(seq_t k)
{
    if (cache_len == cache_cap)
    {
        size_t new_cap = cache_cap == 0 ? 8 : cache_cap * 2;
        cache = seq_realloc(cache, cache_len, new_cap);
        cache_cap = new_cap;
    }
    cache[cache_len++] = k;
}

/* Main OEIS sequence function. */
seq_t a(idx_t n)
{
    /* Generate terms until we have the n‑th. */
    while (cache_len < n)
    {
        seq_t k = (seq_t)next_candidate;
        if (passes_condition(k))
            cache_push(k);
        next_candidate++;
    }
    return cache[n - 1];
}
