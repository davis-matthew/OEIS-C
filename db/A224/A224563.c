/*
 * A224563  Number of (n+3) X 6 0..1 matrices with each 4 X 4 subblock
 *          idempotent.
 * https://oeis.org/A224563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 20, u32 n <= 54, u64 n <= 123, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A224563: number of (n+3)×6 0..1 matrices with each 4×4 subblock
 * idempotent.
 *
 * Recurrence for n > 10:
 *   a(n) = 3*a(n-1) - 2*a(n-2) - 2*a(n-3) + 4*a(n-4)
 *          - 3*a(n-5) + 2*a(n-7) - a(n-8)
 *
 * Initial values (a(1) … a(10)):
 *   512, 428, 514, 629, 728, 929, 1232, 1627, 2095, 2739
 *
 * The function memoises results so that calls are fast and can arrive in
 * any order.
 */

#include <stddef.h>
#include <stdlib.h>

/* Ensure the dynamic array can hold at least 'req' elements. */
static void grow_seq_array(seq_t **arr, size_t *cap, size_t req)
{
    if (*cap >= req) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < req) new_cap <<= 1;
    *arr = (seq_t *)realloc(*arr, new_cap * sizeof(**arr));
    *cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset is 1 – safety guard */

    /* Static cache state */
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static size_t length   = 0;

    /* Make sure the cache can hold a(n) */
    size_t required = (size_t)n;                 /* n is uint64_t, fits in size_t up to 10 000 */
    grow_seq_array(&cache, &capacity, required);

    /* Known initial values */
    static const seq_t init[10] = {
        512, 428, 514, 629, 728, 929, 1232, 1627, 2095, 2739
    };

    /* Compute any missing terms */
    for (size_t i = length; i < required; ++i) {          /* i is 0‑based index of term a(i+1) */
        seq_t val;
        size_t term = i + 1;                             /* the n being computed */

        if (term <= 10) {
            val = init[term - 1];
        } else {
            /* Use the recurrence.  All referenced terms are already in cache. */
            seq_t t1 = cache[i - 1];
            seq_t t2 = cache[i - 2];
            seq_t t3 = cache[i - 3];
            seq_t t4 = cache[i - 4];
            seq_t t5 = cache[i - 5];
            seq_t t7 = cache[i - 7];
            seq_t t8 = cache[i - 8];
            val = 3 * t1 - 2 * t2 - 2 * t3 + 4 * t4 - 3 * t5 + 2 * t7 - t8;
        }
        cache[i] = val;
    }

    length = required;
    return cache[n - 1];
}
