/*
 * A039062  Numbers whose base-8 representation has the same number of 2's and
 *          3's.
 * https://oeis.org/A039062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n‑th integer whose base‑8 representation contains an
 *            equal number of digits 2 and 3
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
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

static bool has_equal_2_3(seq_t x)
{
    unsigned c2 = 0, c3 = 0;
    while (x > 0) {
        unsigned d = (unsigned)(x % 8);
        if (d == 2) ++c2;
        else if (d == 3) ++c3;
        x /= 8;
    }
    return c2 == c3;
}

static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_len  = 0;
static seq_t next_search = 0;

seq_t a(idx_t n)
{
    if (n <= 0)
        return 0;                     /* offset starts at 1 */

    /* Ensure cache can hold at least n entries */
    if ((size_t)n > cache_cap)
        grow_seq(&cache, &cache_cap, (size_t)n);

    /* Generate terms until we reach n */
    while (cache_len < (size_t)n) {
        /* Find next candidate with equal 2s and 3s */
        seq_t cand = next_search;
        while (!has_equal_2_3(cand))
            ++cand;

        /* Store it */
        cache[cache_len++] = cand;
        next_search = cand + 1;        /* start searching from the next integer */
    }

    return cache[n - 1];
}
