/*
 * A032875  Numbers whose base-4 representation Sum_{i=0..m} d(i)*4^i has d(0)
 *          > d(1) < d(2) > ...
 * https://oeis.org/A032875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n‑th integer whose base‑4 digits alternate
   d0 > d1 < d2 > d3 < …  (least significant digit first). */

#include <stdlib.h>
#include <stdbool.h>

/* test whether x satisfies the alternating base‑4 condition */
static bool good(seq_t x)
{
    seq_t last = x % 4;
    x = x / 4;
    int pos = 0;                      /* position of 'last' (0‑based) */
    while (x > 0) {
        seq_t next = x % 4;
        x = x / 4;
        if ((pos & 1) == 0) {         /* need last > next */
            if (!(last > next))
                return false;
        } else {                      /* need last < next */
            if (!(last < next))
                return false;
        }
        last = next;
        ++pos;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                    /* undefined for n<1, harmless */

    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;
    static seq_t candidate   = 1;       /* next integer to test */

    while (cache_len < (size_t)n) {
        /* find the next integer that satisfies the rule */
        while (!good(candidate))
            ++candidate;

        /* grow cache if needed */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache   = tmp;
            cache_cap = new_cap;
        }

        cache[cache_len++] = candidate;
        ++candidate;
    }

    return cache[n - 1];
}
