/*
 * A008520  Numbers whose American English name contains the letter 'e'.
 * https://oeis.org/A008520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   A008520. The nth number whose American English name contains the
 *            letter 'e'. The function builds the sequence lazily; it does not
 *            rely on any external tables
 * Range:     u8 n <= 237, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 9720 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static bool contains_e(idx_t n)
{
    /* name 'zero' contains e, so 0 is always included. */
    if (n == 0)
        return true;

    /* 0..9                                 0 1 2 3 4 5 6 7 8 9
       unit_has_e[0] = true   (zero has 'e')
       unit_has_e[1] = true   (one has 'e')
       unit_has_e[2] = false  (two has no 'e')
       unit_has_e[3] = true   (three has 'e')
       unit_has_e[4] = false  (four has no 'e')
       unit_has_e[5] = true   (five has 'e')
       unit_has_e[6] = false  (six no 'e')
       unit_has_e[7] = true   (seven has 'e')
       unit_has_e[8] = true   (eight has 'e')
       unit_has_e[9] = true   (nine has 'e')
    */
    static const bool unit_has_e[10] = {
        true,  /* 0 zero */
        true,  /* 1 one  */
        false, /* 2 two  */
        true,  /* 3 three*/
        false, /* 4 four */
        true,  /* 5 five */
        false, /* 6 six  */
        true,  /* 7 seven*/
        true,  /* 8 eight*/
        true   /* 9 nine */
    };

    /* tens 20..90.
       Index 2 = 'twenty'  (e), 3='thirty' (no), 4='forty'  (no),
       5='fifty'  (no), 6='sixty'  (no), 7='seventy'(e),
       8='eighty' (e), 9='ninety' (e).
    */
    static const bool tens_has_e[10] = {
        false, /* 0 */
        false, /* 1 */
        true,  /* 2  twenty */
        false, /* 3  thirty */
        false, /* 4  forty */
        false, /* 5  fifty */
        false, /* 6  sixty */
        true,  /* 7  seventy */
        true,  /* 8  eighty */
        true   /* 9  ninety */
    };

    while (n > 0) {
        idx_t group = n % 1000U;   /* 0..999 */
        if (group >= 100U)         /* contains the word 'hundred' */
            return true;

        if (group != 0U) {
            if (group < 20U) {
                /* 10-19 all contain 'e', 1-9 handled below */
                if (group >= 10U)
                    return true;
                /* group 1,3,5,7,8,9 have 'e'; we test via unit_has_e */
                if (unit_has_e[group])
                    return true;
            } else {
                /* 20-99: check tens and units */
                idx_t tens_digit = (group / 10U) % 10U;  /* 2..9 */
                idx_t units = group % 10U;
                if (tens_has_e[tens_digit])
                    return true;
                if (units != 0U && unit_has_e[units])
                    return true;
            }
        }
        n /= 1000U;  /* move to next group */
    }
    return false;  /* no part contained the letter 'e' */
}

seq_t a(idx_t n)
{
    if (n == 0)  /* behaviour below the offset is irrelevant */
        return 0;

    /* lazily built cache of terms */
    static seq_t *cache = NULL;
    static idx_t cache_cnt = 0;
    static size_t cache_cap = 0;
    static idx_t next_candidate = 0;  /* next integer to test */

    /* initialise cache once */
    if (cache == NULL) {
        cache_cap = 64;
        cache = mem_new(seq_t, cache_cap);
    }

    /* grow until we have at least n terms */
    while (cache_cnt < n) {
        if (cache_cnt >= cache_cap) {
            cache_cap *= 2;
            cache = mem_resize(seq_t, cache, cache_cap);
        }
        if (contains_e(next_candidate))
            cache[cache_cnt++] = (seq_t)next_candidate;
        next_candidate++;
    }

    return cache[n - 1];
}
