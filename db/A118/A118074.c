/*
 * A118074  Start with 1 and repeatedly reverse the digits and add 41 to get
 *          the next term.
 * https://oeis.org/A118074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1)=1; a(n)=reverse(a(n‑1))+41 (OEIS A118074)
 * Range:     u8 n <= 8, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-03)
 * Helpers:   mem, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/reverse_digits.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t  cap   = 0;
    static idx_t  len   = 0;  /* number of terms already stored */

    if (n == 0)            /* offset starts at 1; behaviour for n<1 is irrelevant */
        return (seq_t)0;

    /* allocate initial cache if needed */
    if (cache == NULL) {
        cap   = 16;
        cache = mem_new(seq_t, (size_t)cap);
        if (cache == NULL) abort();          /* minimal error handling */
    }

    /* grow cache to hold at least n terms */
    if (cap < n) {
        idx_t new_cap = cap ? cap * 2 : 16;
        if (new_cap < n) new_cap = n;
        seq_t *tmp = mem_resize(seq_t, cache, (size_t)new_cap);
        if (tmp == NULL) abort();
        cache = tmp;
        cap   = new_cap;
    }

    /* initialise first term if not already done */
    if (len == 0) {
        cache[0] = (seq_t)1;
        len = 1;
    }

    /* compute any missing terms up to n */
    if (len < n) {
        idx_t i;
        for (i = len; i < n; ++i) {
            seq_t prev = cache[(size_t)(i - 1)];
            seq_t rev  = reverse_digits(prev);
            cache[(size_t)i] = rev + (seq_t)41;
        }
        len = n;
    }

    return cache[(size_t)(n - 1)];
}
