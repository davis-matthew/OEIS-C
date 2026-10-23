/*
 * A010743  Shifts 4 places left under inverse binomial transform.
 * https://oeis.org/A010743
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 10, i16 n <= 16, i32 n <= 24, i64 n <= 38, i128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
 *   a(0) = 1, a(1) = 2, a(2) = 4, a(3) = 8;
 *   for n ≥ 4:
 *     a(n) = Σ_{k=0}^{n-4} (-1)^{(n-4)-k} * C(n-4,k) * a(k)
 *
 * This implements A010743: Shifts 4 places left under inverse binomial transform.
 */
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cap   = 0;          /* allocated capacity */
    static size_t next  = 0;          /* next index to compute (already computed up to next-1) */

    /* allocate/expand cache to hold indices up to n */
    if (cache == NULL) {
        cap = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
    } else if (n + 1 > cap) {
        size_t new_cap = cap;
        while (new_cap <= n) new_cap = new_cap ? new_cap * 2 : 16;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        cap = new_cap;
    }

    /* initialization of first four terms */
    if (next == 0) {
        cache[0] = 1;
        cache[1] = 2;
        cache[2] = 4;
        cache[3] = 8;
        next = 4;                 /* next index to compute is 4 */
    }

    /* compute missing values up to n */
    while (next <= n) {
        idx_t i = next;
        if (i < 4) {              /* already initialized */
            next++;
            continue;
        }

        idx_t m = i - 4;             /* length of current inverse binomial sum */
        seq_t sum  = 0;
        seq_t binom = 1;             /* C(m,0) */
        int sign = (m & 1) ? -1 : 1; /* (-1)^m */

        for (idx_t k = 0; k <= m; ++k) {
            seq_t prod = binom * cache[k];
            if (sign < 0) prod = -prod;
            sum += prod;

            if (k < m) {
                binom = (binom * (seq_t)(m - k)) / (seq_t)(k + 1);
                sign = -sign;
            }
        }

        cache[i] = sum;
        next++;
    }

    return cache[n];
}
