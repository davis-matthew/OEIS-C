/*
 * A063882  a(n) = a(n - a(n - 1)) + a(n - a(n - 4)), with a(1) = ... = a(4) =
 *          1.
 * https://oeis.org/A063882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn
 * Formula:   a(n) = a(n - a(n - 1)) + a(n - a(n - 4)); Initial values: a(1) =
 *            a(2) = a(3) = a(4) = 1
 * Range:     u8 n <= 501, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External integer types are provided by the build system */
static seq_t *seq_vals = NULL;   /* 1‑based array of computed values */
static size_t  capacity  = 0;     /* allocated array size (in elements)   */
static idx_t   max_index = 0;     /* highest index computed so far        */

seq_t a(idx_t n)
{
    /* Handle the special initial values directly */
    if (n <= 4) {
        /* Ensure the array has space for index 4 */
        if (max_index < 4) {
            size_t need = 5;                    /* indices 0..4 inclusive */
            if (capacity < need) {
                size_t new_cap = capacity ? capacity << 1 : 16;
                while (new_cap < need) new_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
                if (tmp == NULL) abort();       /* memory allocation failed */
                seq_vals = tmp;
                capacity = new_cap;
            }
            /* Initialize the first four values */
            seq_vals[1] = seq_vals[2] = seq_vals[3] = seq_vals[4] = (seq_t)1;
            max_index = 4;
        }
        return seq_vals[(size_t)n];
    }

    /* Grow the storage if we haven't reached the required index yet */
    if (n > max_index) {
        size_t need = (size_t)n + 1;   /* indices 0..n inclusive */
        if (capacity < need) {
            size_t new_cap = capacity ? capacity << 1 : 16;
            while (new_cap < need) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
            if (tmp == NULL) abort();   /* memory allocation failed */
            seq_vals = tmp;
            capacity = new_cap;
        }

        /* Compute the missing terms up to n */
        for (idx_t i = max_index + 1; i <= n; ++i) {
            if (i <= 4) {
                seq_vals[(size_t)i] = (seq_t)1;
            } else {
                idx_t idx1 = i - (idx_t)seq_vals[(size_t)(i - 1)];
                idx_t idx4 = i - (idx_t)seq_vals[(size_t)(i - 4)];
                seq_vals[(size_t)i] =
                    seq_vals[(size_t)idx1] + seq_vals[(size_t)idx4];
            }
        }
        max_index = n;
    }
    return seq_vals[(size_t)n];
}
