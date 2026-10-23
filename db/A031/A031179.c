/*
 * A031179  Numbers having period-1 3-digitized sequences.
 * https://oeis.org/A031179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm:  a(n) = the n‑th integer that eventually reaches a
 * fixed point under the map f(x) = sum of cubes of its decimal digits.
 * Fixed points are 0, 1, 153, 370, 371, 407.  The sequence is generated
 * lazily: each candidate integer is tested by Floyd’s cycle detection,
 * and the first n qualifying numbers are cached for fast future access.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

static inline seq_t sum_cubes(seq_t v)
{
    seq_t sum = 0;
    while (v != 0) {
        unsigned d = (unsigned)(v % 10);
        seq_t d3 = (seq_t)(d * d * d);
        sum += d3;
        v /= 10;
    }
    return sum;
}

static inline bool is_fixed(seq_t x)
{
    return (x == (seq_t)0)  ||
           (x == (seq_t)1)  ||
           (x == (seq_t)153)||
           (x == (seq_t)370)||
           (x == (seq_t)371)||
           (x == (seq_t)407);
}

/* Return true if the orbit of start reaches a fixed point,
 * or false if it loops in a non‑fixed cycle. */
static bool reaches_fixed(seq_t start)
{
    if (is_fixed(start))
        return true;

    seq_t tortoise = sum_cubes(start);
    seq_t hare     = sum_cubes(sum_cubes(start));

    while (true) {
        if (is_fixed(tortoise) || is_fixed(hare))
            return true;
        if (tortoise == hare)
            return false;          /* cycle detected */
        tortoise = sum_cubes(tortoise);
        hare     = sum_cubes(sum_cubes(hare));
    }
}

/* Sequence a(idx_t n) defined for n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* undefined, return 0 for safety */

    static seq_t *cache            = NULL;
    static size_t cache_len        = 0;
    static size_t cache_cap        = 0;
    static idx_t next_candidate    = 0;

    /* Grow cache if necessary. */
    if (cache_cap < (size_t)n) {
        size_t new_cap = (cache_cap == 0 ? 64 : cache_cap * 2);
        if (new_cap < (size_t)n)
            new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return 0;                  /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        if (reaches_fixed((seq_t)next_candidate))
            cache[cache_len++] = (seq_t)next_candidate;
        ++next_candidate;
    }

    return cache[n - 1];
}
