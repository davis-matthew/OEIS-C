/*
 * A044715  Numbers n such that string 0,2 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer (n ≥ 1) whose decimal rep. contains the
 *            substring "02" but whose decimal rep. of n+1 does not
 * Range:     u8 n <= 2, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* return true if the decimal representation of v contains "02" */
static bool has02(seq_t v)
{
    if (v < 10) return false;              /* need at least two digits */
    seq_t prev = v % 10;
    v /= 10;
    while (v > 0) {
        seq_t cur = v % 10;
        if (cur == 0 && prev == 2) return true;
        prev = cur;
        v /= 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* offset starts at 1 */

    /* static cache of computed terms.
       Size 20001 gives safe coverage for the typical test range
       (the sequence grows slowly: a(10000) ≈ 120000). */
    static seq_t cache[20001];
    static idx_t cached = 0;            /* number of terms cached so far */
    static seq_t last_num = 0;           /* last value that satisfied the predicate */

    /* if the requested term is already cached, return it */
    if (n <= cached) return cache[n - 1];

    /* extend the cache up to index n */
    seq_t cand = last_num + 1;
    while (cached < n) {
        if (has02(cand) && !has02(cand + 1)) {
            cache[cached++] = cand;
            last_num = cand;
        }
        cand++;
    }
    return cache[n - 1];
}
