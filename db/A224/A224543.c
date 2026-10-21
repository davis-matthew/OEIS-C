/*
 * A224543  Number of (n+1) X (n+1) 0..1 matrices with each 2 X 2 subblock
 *          idempotent.
 * https://oeis.org/A224543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A224543
 * Formula: a(n) = 6*a(n-1) - 12*a(n-2) + 5*a(n-3) + 12*a(n-4)
 *                - 12*a(n-5) - 3*a(n-6) + 6*a(n-7) - a(n-9)   for n ≥ 10
 * Offset: 1 (a(1)=8). */
/* This implementation uses memoisation with a dynamic array. */

#include <stdlib.h>
#include <stdbool.h>

static const seq_t init[10] = {0, 8, 32, 78, 196, 428, 916,
                               1858, 3678, 7096};

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Memoisation buffer */
    static seq_t *buf = NULL;
    static idx_t buf_size = 0;
    static idx_t last_calculated = 0;
    static bool base_initialised = false;

    /* Ensure capacity for indices 0…n */
    if (buf_size < n + 1) {
        idx_t new_size = buf_size ? buf_size * 2 : 16;
        while (new_size < n + 1)
            new_size *= 2;
        seq_t *tmp = (seq_t *)realloc(buf, new_size * sizeof(seq_t));
        if (!tmp)               /* allocation failed – give up */
            return 0;
        buf = tmp;
        buf_size = new_size;
    }

    /* Initialise the first 9 values once */
    if (!base_initialised) {
        for (idx_t i = 1; i <= 9 && i < buf_size; ++i)
            buf[i] = init[i];
        last_calculated = buf_size > 0 ? (buf_size - 1 < 9 ? buf_size - 1 : 9) : 0;
        base_initialised = true;
    }

    /* If the requested term is already computed, return it */
    if (n <= last_calculated)
        return buf[n];

    /* Ensure that indices up to 9 are defined before the recurrence */
    if (last_calculated < 9)
        last_calculated = 9;

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = last_calculated + 1; i <= n; ++i) {
        seq_t val = 6 * buf[i - 1];
        val -= 12 * buf[i - 2];
        val += 5 * buf[i - 3];
        val += 12 * buf[i - 4];
        val -= 12 * buf[i - 5];
        val -= 3 * buf[i - 6];
        val += 6 * buf[i - 7];
        val -= buf[i - 9];
        buf[i] = val;
    }

    last_calculated = n;
    return buf[n];
}
