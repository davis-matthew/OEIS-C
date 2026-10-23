/*
 * A039355  Numbers whose base-8 representation has the same nonzero number of
 *          5's and 7's.
 * https://oeis.org/A039355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039355: numbers whose base‑8 representation has the
   same non‑zero number of 5's and 7's.  The function returns the n‑th
   such number (n ≥ 1).  It lazily generates the sequence, stores it
   in a static cache, and returns cached values for subsequent calls.
   The algorithm simply scans successive integers, checks the
   base‑8 digit condition, and records the qualifying numbers.
   The implementation uses only integer arithmetic operations
   allowed by the build environment. */

#include <stdlib.h>
#include <stdbool.h>

/* Return true if x expressed in base 8 has the same positive
   count of digit 5 and digit 7.  The empty 0 representation
   is ignored. */
static bool good_value(seq_t x)
{
    unsigned count5 = 0;
    unsigned count7 = 0;
    seq_t tmp = x;

    while (tmp != 0) {
        unsigned d = (unsigned)(tmp % 8);   /* 0 ≤ d ≤ 7 */
        if (d == 5) ++count5;
        else if (d == 7) ++count7;
        tmp /= 8;
    }
    return (count5 == count7) && (count5 > 0);
}

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next_to_test = 1;   /* next integer to examine */

    if (n == 0) return 0;  /* offset is 1; behaviour below is irrelevant */

    size_t target = (size_t)n;

    /* Extend the cache until it contains at least `target` terms. */
    while (cache_len < target) {
        if (good_value(next_to_test)) {
            /* Grow cache if necessary. */
            if (cache_len >= cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : (cache_cap << 1);
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                cache  = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_to_test;
        }
        ++next_to_test;
    }

    return cache[target - 1];
}
