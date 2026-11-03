/*
 * A043477  Numbers having one 6 in base 9.
 * https://oeis.org/A043477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers having one 6 in base 9: generate by scanning integers and
   selecting those whose base-9 representation contains exactly one digit 6. */
#include <stdlib.h>
#include "../utils/mem.h"

static int has_one_6(idx_t x)
{
    int count = 0;
    while (x) {
        if ((x % (idx_t)9) == (idx_t)6) {
            ++count;
            if (count > 1) return 0;
        }
        x /= (idx_t)9;
    }
    return count == 1;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;
    static idx_t next = 1;          /* next integer to test */

    if (n == 0) return 0; /* offset 1; undefined for n<1 */

    /* ensure cache is large enough to hold n terms */
    if (len < n) {
        if (cap == 0) {
            cap = 8;
            cache = mem_new(seq_t, cap);
            if (!cache) abort();
        }
        /* generate terms until we have at least n */
        while (len < n) {
            if (has_one_6(next)) {
                if (len >= cap) {
                    size_t new_cap = cap * 2;
                    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                    if (!tmp) abort();
                    cache = tmp;
                    cap = new_cap;
                }
                cache[len++] = (seq_t)next;
            }
            ++next;
        }
    }
    return cache[n-1];
}
