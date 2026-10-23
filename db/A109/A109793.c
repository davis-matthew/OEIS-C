/*
 * A109793  a(n) = - a(n-1) - a(n-2) - a(n-3) + 50*(n-4) + 50*a(n-5) +
 *          50*a(n-6) + 50*a(n-7), n >= 8.
 * https://oeis.org/A109793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            50*a(n-4)+50*a(n-5)+50*a(n-6)+50*a(n-7)-a(n-1)-a(n-2)-a(n-3)
 *            initial values: a(0)=1 a(1)=1 a(2)=1 a(3)=1 a(4)=1 a(5)=5
 *            a(6)=25
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 25, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static const seq_t init_vals[7] = {1, 1, 1, 1, 1, 5, 25};

static seq_t *memo = NULL;          /* array of computed terms */
static idx_t capacity = 0;          /* allocated capacity (size in elements) */
static idx_t size      = 0;          /* number of terms that have been computed */

static void ensure_capacity(idx_t n)
{
    if (capacity <= n) {
        idx_t new_cap = (capacity == 0) ? 8 : capacity;
        while (new_cap <= n) new_cap <<= 1;      /* double until large enough */

        seq_t *new_ptr = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
        memo = new_ptr;
        capacity = new_cap;

        /* initialise new entries: earlier values already set */
        for (idx_t i = size; i < capacity; ++i) {
            if (i < 7)
                memo[i] = init_vals[i];
            else
                memo[i] = 0;
        }
        /* update size to at least the number of initial terms */
        if (size < 7) size = 7;
    }
}

/* compute seq_t a(idx_t n) for any n >= 0 */
seq_t a(idx_t n)
{
    if (n < 7) {
        return init_vals[n];
    }

    if (n >= size) {
        ensure_capacity(n);
        for (idx_t i = size; i <= n; ++i) {
            /* recurrence for i >= 7 */
            seq_t term = (seq_t)50 * memo[i-4];
            term += (seq_t)50 * memo[i-5];
            term += (seq_t)50 * memo[i-6];
            term += (seq_t)50 * memo[i-7];
            term -= memo[i-1];
            term -= memo[i-2];
            term -= memo[i-3];
            memo[i] = term;
        }
        size = n + 1;
    }

    return memo[n];
}
