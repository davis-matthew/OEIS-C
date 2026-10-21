/*
 * A084345  Numbers with a nonprime number of 1's in their binary expansion
 *          (complement of A052294).
 * https://oeis.org/A084345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 108, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = nth integer having a non‑prime number of 1’s in its binary
 * expansion (the complement of A052294). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_int.h"

/* popcount for seq_t: clear lowest set bit repeatedly */
static unsigned int popcount_seq(seq_t x)
{
    unsigned int cnt = 0;
    while (x) {
        cnt++;
        x &= x - 1;          /* clear lowest set bit */
    }
    return cnt;
}

seq_t a(idx_t n)
{
    /* static cache of accepted numbers */
    static seq_t *cache = NULL;
    static size_t   cap   = 0;
    static size_t   len   = 0;
    static idx_t    next_candidate = 0;

    if (cache == NULL) {          /* first call: initialize */
        cap = 64;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* generate terms until we have at least n entries */
    while (len < n) {
        seq_t x = (seq_t)next_candidate;
        unsigned int pc = popcount_seq(x);
        bool ok = (pc <= 1) || !is_prime_int(pc);   /* non‑prime popcount */

        if (ok) {
            if (len >= cap) {
                cap *= 2;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[len++] = x;
        }
        next_candidate++;
    }

    return cache[n - 1];
}
