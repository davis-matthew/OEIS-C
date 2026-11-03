/*
 * A033675  Trajectory of 166 under map x->x + (x-with-digits-reversed).
 * https://oeis.org/A033675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 5, u32 n <= 16, u64 n <= 43, u128 n <= 87,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 896 (2026-11-03)
 * Helpers:   mem, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A033675: a(0) = 166; a(n+1) = a(n) + rev_decimal(a(n)) */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/reverse_digits.h"
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

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* array of a values */
    static idx_t size = 0;          /* number of terms computed */
    static idx_t cap  = 0;          /* allocated capacity of cache */

    /* Expand cache to at least n+1 elements */
    if (size <= n)
    {
        grow_seq(&cache, &cap, (size_t)(n + 1));

        for (idx_t i = size; i <= n; ++i)
        {
            if (i == 0)
                cache[0] = (seq_t)166;
            else
                cache[i] = cache[i - 1] + reverse_digits(cache[i - 1]);
        }
        size = n + 1;
    }

    return cache[n];
}
