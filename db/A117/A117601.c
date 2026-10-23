/*
 * A117601  Padovan numbers which are divisible by the sum of their digits.
 * https://oeis.org/A117601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 15, u32 n <= 19, u64 n <= 23, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Padovan numbers P(n),  P(0)=P(1)=P(2)=1, P(n)=P(n‑2)+P(n‑3),
   filtered to keep only terms divisible by the sum of their decimal digits. */

/* The function returns a(1)=1, a(2)=2, a(3)=3, ... as listed by OEIS A117601. */

#include <stdlib.h>
#include <stdbool.h>

/* Return the decimal digit sum of a seq_t value. */
static seq_t digit_sum(seq_t v)
{
    seq_t sum = 0;
    while (v > 0) {
        sum += v % 10;
        v /= 10;
    }
    return sum;
}

/* Append a value to a dynamic array, expanding it if necessary. */
static void append_cache(seq_t **cache, size_t *size, size_t *cap, seq_t val)
{
    if (*size == *cap) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
        /* In C++ realloc returns void*; cast to the correct pointer type. */
        seq_t *tmp = (seq_t *)realloc(*cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            abort();                      /* unrecoverable memory exhaustion */
        }
        *cache = tmp;
        *cap   = new_cap;
    }
    (*cache)[(*size)++] = val;
}

/* Return the nth term (1‑based) of A117601. */
seq_t a(idx_t n)
{
    /* Memoised cache of found terms. */
    static seq_t *cache   = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;

    /* Current state of Padovan generation. */
    static idx_t padovan_idx = 0;   /* next Padovan index to produce (0‑based) */
    static seq_t p_a = 1;          /* P[i‑3] */
    static seq_t p_b = 1;          /* P[i‑2] */
    static seq_t p_c = 1;          /* P[i‑1] */
    static seq_t last_seen = 0;    /* last distinct Padovan value produced */

    /* Generate Padovan numbers until we have at least n sequence terms. */
    while (cache_size < n) {
        seq_t val;

        if (padovan_idx < 3) {
            /* The first three Padovan numbers are all 1. */
            val = 1;
            padovan_idx++;
        } else {
            /* P[i] = P[i‑2] + P[i‑3] */
            val = p_a + p_b;
            /* Shift the sliding window for the next iteration. */
            seq_t old_a = p_a;
            p_a = p_b;
            p_b = p_c;
            p_c = val;
            padovan_idx++;
        }

        /* Skip duplicate Padovan values (only 1 occurs repeatedly). */
        if (val == last_seen)
            continue;
        last_seen = val;

        /* Check divisibility by digit sum. */
        seq_t sd = digit_sum(val);
        if (sd != 0 && (val % sd == 0)) {
            append_cache(&cache, &cache_size, &cache_cap, val);
        }
    }

    /* Return the requested 1‑based term. */
    return cache[n - 1];
}
