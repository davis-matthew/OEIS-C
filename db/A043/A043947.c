/*
 * A043947  Numbers k such that 2 and 5 occur juxtaposed in the base-8
 *          representation of k but not of k+1.
 * https://oeis.org/A043947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers k whose base-8 representation contains 25 or 52, * but
 *            the representation of k+1 does not. * The function a(n) returns
 *            the nth such number (offset 1)
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Check if an octal number contains the adjacent digits 2 and 5
 * (in either order). */
static bool has_adjacent_25_or_52(seq_t x)
{
    int prev = -1;
    while (x > 0) {
        int d = (int)(x % 8);
        if ((prev == 2 && d == 5) || (prev == 5 && d == 2))
            return true;
        prev = d;
        x = x / 8;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* offset starts at 1; 0 is a safe dummy value */

    static seq_t *cache = NULL;      /* cached terms */
    static size_t cache_size = 0;    /* current number of cached terms */
    static size_t cache_cap   = 0;    /* capacity of the cache array */
    static seq_t next_candidate = 0;  /* next number to test */

    /* Generate terms until we have at least n of them. */
    while (cache_size < (size_t)n) {
        next_candidate++;
        if (has_adjacent_25_or_52(next_candidate) &&
            !has_adjacent_25_or_52(next_candidate + 1)) {

            /* Resize cache if needed. */
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 1024;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp)
                    exit(EXIT_FAILURE);   /* memory allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = next_candidate;
        }
    }

    return cache[n - 1];
}
