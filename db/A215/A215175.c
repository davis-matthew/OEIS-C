/*
 * A215175  a(2*n) = A214297(n), a(2*n+1) = A000290(n).
 * https://oeis.org/A215175
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 24, i16 n <= 55, i32 n <= 55, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A215175: a(2*n) = A214297(n), a(2*n+1) = A000290(n).
   Recurrence: a(n) = 2*a(n-2) - a(n-4) + a(n-8) - 2*a(n-10) + a(n-12) for n ≥ 13.
   Initial values for n = 0 .. 12:
   -1, 0, 0, 1, -3, 4, 2, 9, 3, 16, 6, 25, 5
*/

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

static seq_t *A_cache = NULL;          /* cache for a(n) values   */
static idx_t  A_cache_size = 0;        /* number of valid entries */
static size_t A_cache_cap   = 0;        /* allocated capacity       */

static const seq_t A_init_vals[13] = {
    -1, 0, 0, 1, -3, 4, 2, 9, 3, 16, 6, 25, 5
};

static void ensure_cache_capacity(idx_t needed)
{
    if (needed < A_cache_size) return;
    size_t required = (size_t)(needed + 1);
    if (required <= A_cache_cap) return;

    size_t new_cap = A_cache_cap ? A_cache_cap : 16;
    while (new_cap < required) new_cap <<= 1;

    seq_t *new_arr = (seq_t *)realloc(A_cache, new_cap * sizeof(seq_t));
    if (!new_arr) exit(EXIT_FAILURE); /* allocation failed */

    A_cache = new_arr;
    A_cache_cap = new_cap;
}

static void compute_until(idx_t k)
{
    if (k < A_cache_size) return;

    ensure_cache_capacity(k);
    for (idx_t i = A_cache_size; i <= k; ++i) {
        /* recurrence needs indices ≥ 13; for i < 13 values are already set */
        seq_t val = (seq_t)2 * A_cache[i - 2];
        val -= A_cache[i - 4];
        val += A_cache[i - 8];
        val -= (seq_t)2 * A_cache[i - 10];
        val += A_cache[i - 12];
        A_cache[i] = val;
    }
    A_cache_size = k + 1;
}

seq_t a(idx_t n)
{
    if (A_cache == NULL) {
        size_t init_cap = 16;
        A_cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!A_cache) exit(EXIT_FAILURE);
        for (size_t i = 0; i < 13; ++i)
            A_cache[i] = A_init_vals[i];
        A_cache_size = 13;
        A_cache_cap   = init_cap;
    }

    if (n >= A_cache_size) compute_until(n);
    return A_cache[n];
}
