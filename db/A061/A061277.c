/*
 * A061277  Numbers which have more different digits than their squares.
 * https://oeis.org/A061277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th integer x with more distinct decimal digits than
 *            its square, i.e. distinct_digits(x) > distinct_digits(x^2)
 * Range:     u8 none, u16 n <= 3, u32 n <= 2693, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 60 terms through n = 9915 (2026-11-03)
 * Helpers:   digit_counts, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/digit_counts.h"
#include "../utils/mem.h"

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

/* Count the number of distinct decimal digits of a value of type seq_t. */
static inline int distinct_digits(seq_t v)
{
    unsigned char cnt[10] = {0};
    digit_counts(v, cnt);
    int d = 0;
    for (int i = 0; i < 10; ++i)
        if (cnt[i]) ++d;
    return d;
}

/* Return the n-th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache of found terms               */
    static size_t cache_cap = 0;  /* capacity of the cache             */
    static size_t cache_len = 0;  /* number of terms stored so far     */
    static seq_t next = 1;        /* next integer to test              */

    /* Extend the cache until it contains at least n terms. */
    while (cache_len < (size_t)n)
    {
        int d1 = distinct_digits(next);
        seq_t square = next * next;
        int d2 = distinct_digits(square);

        if (d1 > d2)
        {
            /* Resize cache if necessary. */
            if (cache_len + 1 > cache_cap)
                grow_seq(&cache, &cache_cap, cache_len + 1);

            cache[cache_len++] = next;
        }

        ++next;
    }

    return cache[n - 1];
}
