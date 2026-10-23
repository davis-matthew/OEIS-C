/*
 * A167757  Number of reduced words of length n in Coxeter group on 32
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^15 = I.
 * https://oeis.org/A167757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence a(n) =
 *            30·a(n-1)+30·a(n-2)+⋯+30·a(n-14) – 465·a(n-15) (n ≥ 15), with
 *            initial values a(0)=1, a(1)=32, a(2)=992, a(3)=30752,
 *            a(4)=953312, a(5)=29552672, a(6)=916132832, a(7)=28400117792,
 *            a(8)=880403651552, a(9)=27292513198112, a(10)=846067909141472,
 *            a(11)=262281...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   seq_from_str, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_from_str.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static seq_t init_vals[16];
    static bool init_done = false;

    if (!init_done) {
        /* initialise the first 16 values from literals (safe for any seq_t) */
        const char *const init_str[16] = {
            "1",
            "32",
            "992",
            "30752",
            "953312",
            "29552672",
            "916132832",
            "28400117792",
            "880403651552",
            "27292513198112",
            "846067909141472",
            "26228105183385632",
            "813071260684954592",
            "25205209081233592352",
            "781361481518241362912",
            "24222205927065482249776"
        };
        for (size_t i = 0; i < 16; ++i)
            init_vals[i] = seq_from_str(init_str[i]);
        init_done = true;
    }

    /* grow the cache if necessary */
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = seq_realloc(cache, cache_len, new_len);
        if (!tmp)      /* allocation failed – return zero as a safe fallback */
            return (seq_t)0;
        cache = tmp;

        /* initialise the newly added entries */
        for (size_t i = cache_len; i < new_len; ++i) {
            if (i < 16) {
                cache[i] = init_vals[i];
            } else {
                seq_t sum = 0;
                for (idx_t j = 1; j <= 14; ++j)
                    sum += cache[i - (size_t)j];
                cache[i] = sum * (seq_t)30 - cache[i - 15] * (seq_t)465;
            }
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
