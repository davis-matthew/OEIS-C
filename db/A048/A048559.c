/*
 * A048559  a(n+1) is next smallest nontrivial square beginning with a(n),
 *          initial square is 4.
 * https://oeis.org/A048559
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 4; for n >= 0, a(n+1) is the smallest perfect square >
 *            a(n) whose decimal representation starts with the decimal
 *            representation of a(n) and whose square root does not end in 0
 *            (the "nontrivial" condition)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   pow10, seq_realloc, sqrt_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/pow10.h"     /* pow10(idx_t) */
#include "../utils/sqrt_floor.h"/* sqrt_floor(seq_t) */
#include "../utils/seq_realloc.h"/* seq_realloc(seq_t*,size_t,size_t) */

/* Helper: compute the next term given the previous term. */
static seq_t next_square(seq_t prev)
{
    /* lower bound for roots: must be greater than sqrt(prev) */
    seq_t sqrt_prev = sqrt_floor(prev);
    seq_t min_root = sqrt_prev + 1;   /* strict > sqrt(prev) */

    seq_t candidate_root = min_root;
    /* iterate over possible digit extensions k = 0,1,2,... */
    for (idx_t k = 0; ; ++k) {
        seq_t multiplier = pow10(k);

        /* compute lower and upper bounds for the square */
        seq_t lower = prev * multiplier;
        seq_t upper = (prev + 1) * multiplier;
        seq_t upper_limit = upper - 1;  /* inclusive upper bound */

        /* compute ceil_sqrt(lower) */
        seq_t s = sqrt_floor(lower);
        seq_t root = s;
        if (s * s < lower)
            ++root;          /* ceil */

        /* ensure root > sqrt_prev */
        if (root <= sqrt_prev)
            root = min_root;

        /* adjust root while respecting the upper bound and nontrivial condition */
        while (true) {
            seq_t r2 = root * root;
            if (r2 > upper_limit)               /* no root works for this k */
                break;
            if ((root % 10) != 0)               /* nontrivial root */
                return r2;                       /* found minimal square */
            ++root;                              /* try next root (skip zeros) */
        }
        /* no root found for this k; try next k */
    }
}

/* Memoized sequence array */
static seq_t *cache = NULL;
static size_t cap = 0;

seq_t a(idx_t n)
{
    /* ensure array at least 1 element to store a(0) = 4 */
    if (cap == 0) {
        cache = seq_realloc(NULL, 0, 1);
        cache[0] = (seq_t)4;
        cap = 1;
    }

    /* grow cache to include terms up to n */
    if ((size_t)n >= cap) {
        cache = seq_realloc(cache, cap, (size_t)n + 1);
        for (size_t i = cap; i <= (size_t)n; ++i) {
            cache[i] = next_square(cache[i - 1]);
        }
        cap = (size_t)n + 1;
    }

    return cache[n];
}
