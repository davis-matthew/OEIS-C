/*
 * A225683  Numbers divisible by their first digit squared.
 * https://oeis.org/A225683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A225683: Numbers divisible by their first digit squared. */
/* a(n) is the nth integer k ≥ 1 such that k % (f(k))² == 0, where f(k) is
   the first (most significant) decimal digit of k. */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* stored terms */
    static size_t len = 0;        /* number of terms stored */
    static size_t cap = 0;        /* capacity of cache */
    static idx_t next_start = 1;  /* next candidate to test */

    if (n == 0)
        return 0;                 /* unused; offset 1 */

    /* Generate terms until we have at least n of them */
    while (len < n) {
        idx_t cand = next_start;

        /* Find the next number whose first digit squared divides it */
        while (1) {
            idx_t t = cand;
            while (t >= 10)
                t /= 10;
            idx_t d = t;
            idx_t sq = d * d;
            if (cand % sq == 0)
                break;
            ++cand;
        }

        /* Ensure space in cache */
        if (len == cap) {
            size_t newcap = cap ? cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp)
                abort();
            cache = tmp;
            cap = newcap;
        }

        /* Store the found term */
        cache[len++] = (seq_t)cand;
        next_start = cand + 1;
    }

    return cache[n - 1];
}
