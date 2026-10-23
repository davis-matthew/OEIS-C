/*
 * A043834  Numbers k such that number of runs in the base 4 representation of
 *          k is congruent to 2 mod 5.
 * https://oeis.org/A043834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th integer k such that the number of runs in the *
 *            base‑4 representation of k is congruent to 2 (mod 5)
 * Range:     u8 n <= 54, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* static cache of found terms */
static seq_t *cache = NULL;        /* array of discovered terms                */
static size_t cache_cap = 0;       /* allocated capacity of 'cache'            */
static idx_t cache_len = 0;        /* number of terms currently stored         */
static seq_t next_search = 1;      /* next integer to test                     */

/* Count the number of runs in the base‑4 representation of v.
 * A run is a maximal consecutive block of identical digits. */
static int count_runs_base4(seq_t v)
{
    if (v == 0) return 1;                 /* base‑4 "0" has one run */
    int runs = 0;
    seq_t prev = (seq_t)4;                /* value outside 0‑3     */
    seq_t x = v;
    while (x > 0) {
        seq_t d = x % (seq_t)4;
        if (d != prev) {
            ++runs;
            prev = d;
        }
        x /= (seq_t)4;
    }
    return runs;
}

/* Return the n‑th term (n ≥ 1).  Calls may be made in any order. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for offset 1, unused */

    /* Expand the cache until it contains the n‑th term. */
    while ((idx_t)cache_len < n) {
        seq_t cand = next_search;
        if (count_runs_base4(cand) % 5 == 2) {
            /* ensure sufficient capacity */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache,
                                              new_cap * sizeof(seq_t));
                if (!tmp) return 0;              /* allocation failure   */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }
        next_search = cand + 1;
    }

    return cache[n - 1];
}
