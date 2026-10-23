/*
 * A167161  The Catapult Sequence.
 * https://oeis.org/A167161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 186, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Catapult sequence: A167161.  The function simulates the
   catapult process: at step n the element a[n] catapults the
   neighbor to its right a[n] positions further to the right.
   The permutation is built incrementally and memoised, so any
   call order is supported. */

#include <stdlib.h>

static seq_t *order_arr    = NULL;   /* current permutation */
static size_t  order_cap   = 0;      /* allocated capacity */
static idx_t   processed   = 0;      /* number of steps already done */

static void ensure_capacity(idx_t max_index)
{
    /* Ensure derivable indices 0..max_index exist in order_arr. */
    size_t needed = (size_t)(max_index + 1);
    if (needed <= order_cap) return;

    size_t new_cap = needed * 2;
    if (new_cap <= needed) new_cap = needed;   /* avoid overflow */

    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_arr) return;  /* out of memory; caller will fail */

    /* Copy existing elements. */
    for (size_t i = 0; i < order_cap; ++i) new_arr[i] = order_arr[i];
    /* Initialise new positions to their natural numeric values. */
    for (size_t i = order_cap; i < new_cap; ++i) new_arr[i] = (seq_t)i;

    free(order_arr);
    order_arr = new_arr;
    order_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Advance the simulation to step n (if needed). */
    for (idx_t step = processed; step <= n; ++step) {
        /* Ensure the neighbor at step+1 exists. */
        ensure_capacity(step + 1);

        seq_t a_step = order_arr[step];          /* catapult strength */
        idx_t new_index = step + 1 + (idx_t)a_step;  /* target index */

        /* Ensure target index exists before shifting. */
        ensure_capacity(new_index);

        seq_t x = order_arr[step + 1];          /* element to move */

        /* Shift elements between step+2 and new_index left by one. */
        if (new_index > step + 1) {
            for (idx_t i = step + 1; i < new_index; ++i)
                order_arr[i] = order_arr[i + 1];
        }

        /* Place the catapulted element at its new position. */
        order_arr[new_index] = x;

        processed = step + 1;                   /* next step ready */
    }

    return order_arr[n];
}
