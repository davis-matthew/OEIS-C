/*
 * A118299  Start with 24 and repeatedly reverse the digits and add 1 to get
 *          the next term.
 * https://oeis.org/A118299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = reverse_decimal(a(n-1)) + 1, a(1) = 24
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Validated: partial, pass, 94 terms through n = 94 (2026-11-01)
 * Helpers:   reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/reverse_digits.h"

/* Grow a dynamic array when needed.  The pointer is passed as (void **). */
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache of computed terms */
    static size_t cap = 0;        /* allocated capacity */
    static idx_t computed = 0;    /* highest index already computed */

    if (n == 0) return 0;         /* below the offset – arbitrary */

    /* Ensure space for terms up to n */
    if ((size_t)n > cap)
        ensure_capacity((size_t)n, &cap, (void **)&cache, sizeof(seq_t));

    /* Return cached value if available */
    if (n <= computed)
        return cache[(size_t)(n - 1)];

    /* Compute missing terms up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1) {
            val = 24;
        } else {
            val = reverse_digits(cache[(size_t)(i - 2)]) + 1;
        }
        cache[(size_t)(i - 1)] = val;
    }
    computed = n;
    return cache[(size_t)(n - 1)];
}
