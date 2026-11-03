/*
 * A039142  Numbers whose base-10 representation has the same number of 4's
 *          and 6's.
 * https://oeis.org/A039142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose base-10 representation has the same number of 4's
 * and 6's.  The function generates the sequence by a brute‑force search
 * with memoisation.  It works with any seq_t (uint8_t…uint64_t,
 * arbitrary‑precision class) and idx_t (uint64_t). */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* test whether a decimal number has equal numbers of 4's and 6's */
static bool equal_4_6(seq_t x)
{
    uint64_t c4 = 0, c6 = 0;
    while (x > 0) {
        int d = (int)(x % 10);
        if (d == 4) {
            ++c4;
        } else if (d == 6) {
            ++c6;
        }
        x /= 10;
    }
    return c4 == c6;
}

/* a(n) returns the nth term of the sequence, with offset 1 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* below offset – not used, but harmless */

    /* static cache for memoisation */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;
    static seq_t cur         = 0;   /* current integer being examined */

    /* grow the cache until it holds at least n elements */
    while (cache_len < n) {
        if (equal_4_6(cur)) {
            if (cache_len >= cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp     = mem_resize(seq_t, cache, new_cap);
                if (!tmp) abort();          /* allocation failure */
                cache     = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cur;
        }
        ++cur;
    }

    return cache[n - 1];
}
