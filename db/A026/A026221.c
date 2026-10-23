/*
 * A026221  Duplicate of A026214.
 * https://oeis.org/A026221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = position of n in the permutation A026214, where odd
 *            numbers 2k−1 occupy positions 3k−1, and each even number 2k is
 *            placed at position k if that position is free and not reserved
 *            for an odd number (positions 3m−1); otherwise it occupies
 *            position 3k
 * Range:     i8 n <= 63, i16 n <= 63, i32 n <= 63, i64 n <= 63, i128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

static seq_t *pos_of_value = NULL;      /* pos_of_value[v] = position of value v   */
static size_t pos_of_value_cap = 0;     /* allocated capacity of pos_of_value      */
static char *occupied = NULL;           /* occupied[p] == 1 if position p is used */
static size_t occupied_cap = 0;         /* allocated capacity of occupied         */
static idx_t max_generated = 0;         /* largest value for which the position is known */

/* Ensure pos_of_value has capacity >= n+1 (index 0 unused). */
static void ensure_pos_of_value(size_t n)
{
    if (pos_of_value_cap <= n) {
        size_t new_cap = pos_of_value_cap ? pos_of_value_cap : 128;
        while (new_cap <= n) new_cap <<= 1;
        pos_of_value = (seq_t *)realloc(pos_of_value, new_cap * sizeof(seq_t));
        /* initialise new slots to 0 */
        memset(pos_of_value + pos_of_value_cap, 0,
               (new_cap - pos_of_value_cap) * sizeof(seq_t));
        pos_of_value_cap = new_cap;
    }
}

/* Ensure occupied has size >= n+1. */
static void ensure_occupied(size_t n)
{
    if (occupied_cap <= n) {
        size_t new_cap = occupied_cap ? occupied_cap : 128;
        while (new_cap <= n) new_cap <<= 1;
        occupied = (char *)realloc(occupied, new_cap);
        memset(occupied + occupied_cap, 0,
               (new_cap - occupied_cap));
        occupied_cap = new_cap;
    }
}

/* Generate the sequence up to value n (inclusive). */
static void generate_upto(idx_t n)
{
    if (n <= max_generated) return;

    ensure_pos_of_value((size_t)n);

    for (idx_t v = max_generated + 1; v <= n; ++v) {
        idx_t pos;   /* 1‑based position, fits in idx_t */

        if (v & 1) {                 /* odd number 2k-1 */
            idx_t k = (v + 1) / 2;
            pos = 3 * k - 1;
        } else {                     /* even number 2k */
            idx_t k = v / 2;
            idx_t target = k;        /* try position k */

            /* make sure we can test occupied[target] */
            ensure_occupied((size_t)target);

            if ((target % 3 == 2) || occupied[target]) {
                /* position reserved or already taken – use 3k */
                target = 3 * k;
            }
            pos = target;
        }

        /* record the position */
        pos_of_value[v] = (seq_t)pos;

        /* mark the position as occupied */
        ensure_occupied((size_t)pos);
        occupied[pos] = 1;
    }
    max_generated = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* the offset is 1; 0 is unused */

    generate_upto(n);
    return pos_of_value[n];
}
