/*
 * A044762  Numbers n such that string 4,9 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth positive integer whose decimal representation *
 *            contains the substring "49" but whose successor n+1 does not
 * Range:     u8 n <= 3, u16 n <= 711, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 55 terms through n = 999 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return true iff x contains the adjacent digits 4 followed by 9. */
static bool contains_4_9(seq_t x)
{
    if (x < 49) /* quick rejection */
        return false;
    int prev = -1;
    while (x > 0) {
        int d = (int)(x % 10);
        if (d == 4 && prev == 9)
            return true;
        prev = d;
        x /= 10;
    }
    return false;
}

/* Sequence a: sequence of numbers satisfying the above property.
 * Indexing follows OEIS offset: a(1) is the first term (49). */
seq_t a(idx_t n)
{
    if (n == 0)  /* offset is 1; caller should never ask for n==0 */
        return 0;

    /* static state for memoisation */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;
    static seq_t next   = 49;

    /* Grow cache until it contains at least n terms */
    while (len < n) {
        /* Test current candidate */
        if (contains_4_9(next) && !contains_4_9(next + 1)) {
            if (len == cap) {
                size_t new_cap = cap == 0 ? 16 : cap * 2;
                void *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) {          /* allocation failed */
                    exit(EXIT_FAILURE);
                }
                cache = (seq_t *)tmp;
                cap   = new_cap;
            }
            cache[len++] = next;
        }
        next += 1;
    }
    return cache[n - 1];  /* OEIS offset 1 */
}
