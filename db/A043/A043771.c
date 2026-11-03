/*
 * A043771  Number of runs in the base 2 representation of n is congruent to 8
 *          mod 10.
 * https://oeis.org/A043771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer whose binary representation has
 *            a number of runs congruent to 8 (mod 10). The implementation
 *            searches integers incrementally and caches results
 * Range:     u8 n <= 1, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static int runs_mod10(seq_t x)
{
    if (x == 0) return 1; /* binary "0" has one run */
    int runs = 1;
    seq_t bit = x & 1;
    x >>= 1;
    while (x != 0) {
        seq_t next_bit = x & 1;
        if (next_bit != bit) runs++;
        bit = next_bit;
        x >>= 1;
    }
    return runs % 10;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset starts at 1 */

    static seq_t *cache = NULL; /* stored numbers satisfying condition */
    static size_t cap   = 0;    /* allocated capacity of cache */
    static size_t len   = 0;    /* number of stored values */
    static idx_t next   = 1;    /* next integer to test */

    while (len < n) {
        /* Find the next integer whose run count is ≡ 8 (mod 10) */
        while (runs_mod10((seq_t)next) != 8)
            ++next;

        /* Grow cache if necessary */
        if (len == cap) {
            size_t new_cap = (cap == 0) ? 16 : cap << 1;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp) abort(); /* allocation failure */
            cache = tmp;
            cap   = new_cap;
        }

        cache[len++] = (seq_t)next;
        ++next; /* advance for next search */
    }

    return cache[n - 1];
}
