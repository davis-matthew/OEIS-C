/*
 * A194548  Triangle read by rows: T(n,k) = number of parts in the k-th
 *          partition of n that does not contain 1 as a part, with partitions
 *          in lexicographic order.
 * https://oeis.org/A194548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 9739, u16 n <= 9739, u32 n <= 9739, u64 n <= 9739,
 *            u128 n <= 9739, big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A194548: Triangle read by rows – T(n,k) is the number of parts of
    the k‑th partition of n into parts ≥ 2, written lexicographically
    (ascending).  a(n) is the n‑th entry of the flattened triangle.   */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_realloc.h"

/* state kept between calls */
static seq_t *data = NULL;      /* array of already produced values */
static size_t size   = 0;       /* number of values stored */
static size_t capacity = 0;     /* current allocation capacity */
static idx_t  last_row = 0;     /* highest n for which a row has been generated */

/* grow the array to accommodate at least one more element */
static void make_room(void)
{
    if (size == capacity) {
        size_t new_cap = capacity ? capacity * 2 : 16;
        data = seq_realloc(data, capacity, new_cap);
        capacity = new_cap;
    }
}

/* append a new value to the array */
static void append(seq_t val)
{
    make_room();
    data[size++] = val;
}

/* recursive generation of partitions of 'rem' into parts >=2,
   where every part is at least 'min_part' (to enforce non‑decreasing order),
   and 'depth' is the current number of parts.  'produced' is set to true
   when at least one partition has been recorded (always true for n>=2). */
static void rec_parts(idx_t rem, idx_t min_part, idx_t depth, bool *produced)
{
    if (rem == 0) {
        append((seq_t)depth);
        *produced = true;
        return;
    }
    for (idx_t part = min_part; part <= rem; ++part) {
        rec_parts(rem - part, part, depth + 1, produced);
    }
}

/* the required public accessor */
seq_t a(idx_t n)
{
    /* generate rows until we have produced at least n values */
    while (size < n) {
        ++last_row;
        if (last_row == 1) {
            /* no partitions of 1 without 1‑parts – store a single zero */
            append((seq_t)0);
        } else {
            bool produced = false;
            rec_parts(last_row, (idx_t)2, (idx_t)0, &produced);
            /* for n>=2 there is always at least one partition */
            (void)produced;          /* silence unused‑variable warning */
        }
    }
    return data[n - 1];
}
