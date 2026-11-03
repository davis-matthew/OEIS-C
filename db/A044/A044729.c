/*
 * A044729  Numbers n such that string 1,6 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer whose decimal representation *
 *            contains the substring "16" but whose successor does not. This
 *            is * computed by scanning numbers sequentially and caching the
 *            results
 * Range:     u8 n <= 4, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-03)
 * Helpers:   get_digits, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include <stdlib.h>
#include "../utils/get_digits.h"
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

/* Return true if the decimal representation of x contains the substring
 * "16".  get_digits outputs the least‑significant digit first, so the
 * substring "16" appears as the pattern 6 followed by 1 in that order. */
static bool has16(seq_t x)
{
    uint8_t buf[128];
    size_t len;
    get_digits(x, buf, &len);
    for (size_t i = 0; i + 1 < len; ++i)
        if (buf[i] == 6 && buf[i + 1] == 1)
            return true;
    return false;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;  /* cached values */
    static size_t cap   = 0;     /* current capacity */
    static size_t sz    = 0;     /* number of cached values */
    static seq_t last_candidate = 0;  /* last integer examined */

    while (sz < n) {
        /* examine the next integer */
        seq_t cand = last_candidate + 1;
        last_candidate = cand;

        if (!has16(cand))
            continue;  /* candidate does not contain "16" */

        seq_t cand_plus = cand + 1;
        if (has16(cand_plus))
            continue;  /* successor also contains "16" */

        /* store the valid number */
        if (sz == cap)
            grow_seq(&cache, &cap, sz + 1);
        cache[sz++] = cand;
    }

    return cache[n - 1];
}
