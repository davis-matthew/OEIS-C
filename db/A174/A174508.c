/*
 * A174508  Continued fraction expansion for exp( Sum_{n>=1} 1/(n*A086594(n))
 *          ), where A086594(n) = (4+sqrt(17))^n + (4-sqrt(17))^n.
 * https://oeis.org/A174508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  cofr, nonn, easy
 * Formula:   a(0) = 1, a(1) = 7, a(2) = 65, a(3) = 1, a(4) = 535, a(5) =
 *            4353, a(6) = 1, a(7) = 35367, a(8) = 287297 (recurrence) a(n) =
 *            67*a(n-3) - 67*a(n-6) + a(n-9) for n ≥ 9 Memoised for arbitrary
 *            call order
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[9] = {
    (seq_t)1, (seq_t)7, (seq_t)65, (seq_t)1,
    (seq_t)535, (seq_t)4353, (seq_t)1,
    (seq_t)35367, (seq_t)287297
};

static seq_t *cache = NULL;          /* allocated array of seq_t */
static idx_t cache_cap = 0;          /* number of elements allocated */
static idx_t cache_len = 0;          /* first index that is NOT yet computed */

seq_t a(idx_t n)
{
    /* Compute up to `n` if needed */
    if (n >= cache_len) {
        /* Ensure we have enough storage */
        if (n + 1 > cache_cap) {
            idx_t new_cap = cache_cap ? cache_cap * 2 : 8;
            while (new_cap <= n) new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) abort();   /* allow failing fast */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Initialise known first terms if not already present */
        for (idx_t i = cache_len; i < 9 && i <= n; ++i)
            cache[i] = init[i];

        /* Compute remaining terms by the recurrence */
        idx_t start = cache_len > 9 ? cache_len : 9;
        for (idx_t i = start; i <= n; ++i) {
            cache[i] = (seq_t)67 * cache[i-3]
                     - (seq_t)67 * cache[i-6]
                     +       cache[i-9];
        }

        cache_len = n + 1;   /* next index that is not yet known */
    }

    return cache[n];
}
