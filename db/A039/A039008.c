/*
 * A039008  Numbers whose base-5 representation has the same number of 0's and
 *          2's.
 * https://oeis.org/A039008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   numbers whose base‑5 representation has the same number of 0's
 *            and 2's
 * Range:     u8 n <= 98, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9103 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* checks if the base‑5 digits of x contain the same count of 0 and 2 */
static bool matches_base5_0_2(idx_t x)
{
    idx_t c0 = 0, c2 = 0;
    while (x > 0) {
        idx_t d = x % 5;
        if (d == 0)  c0++;
        else if (d == 2) c2++;
        x /= 5;
    }
    return c0 == c2;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined for n=0; return 0 as a safe fallback */
        return (seq_t)0;
    }

    /* ----- caching mechanism ------------------------------------------- */
    static uint64_t *cache = NULL;      /* stored terms (as 64‑bit values) */
    static size_t sz = 0;              /* number of cached terms            */
    static size_t cap = 0;             /* current allocated capacity        */
    static idx_t next_candidate = 1;   /* next integer to test              */

    while (sz < (size_t)n) {
        if (sz == cap) {
            size_t new_cap = cap ? cap * 2ULL : 16ULL;
            uint64_t *tmp = (uint64_t *)realloc(cache, new_cap * sizeof(uint64_t));
            if (!tmp) {               /* allocation failure – unwind */
                return (seq_t)0;
            }
            cache = tmp;
            cap = new_cap;
        }
        if (matches_base5_0_2(next_candidate)) {
            cache[sz++] = (uint64_t)next_candidate;
        }
        next_candidate++;
    }

    /* ----- return requested term --------------------------------------- */
    return (seq_t)cache[n - 1];
}
