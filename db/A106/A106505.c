/*
 * A106505  Ordered and uniqued length of side common to the two angles, one
 *          being the double of the other, of a primitive integer-sided
 *          triangle.
 * https://oeis.org/A106505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th distinct value of s^2 - r^2 with * r < s < 2r
 *            and gcd(r,s)=1, returned in increasing order
 * Range:     u8 none, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   cmp_seq, gcd64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/cmp_seq.h"
#include "../utils/gcd64.h"

/* ------------------------------------------------------------------
 * Simple Euclidean gcd for 64‑bit unsigned integers.
 * ------------------------------------------------------------------ */

/* ------------------------------------------------------------------
 * Global storage for values generated so far.
 * ------------------------------------------------------------------ */
static seq_t *vals = NULL;          /* array of values */
static size_t vals_size = 0;        /* number of stored values */
static size_t vals_cap  = 0;        /* allocated capacity of vals */
static idx_t  max_r_processed = 0;  /* largest r that has been processed */
static int    vals_sorted = 1;      /* flag: array is currently sorted and deduped */

/* ------------------------------------------------------------------
 * Helper to append a value to the array, expanding if necessary.
 * ------------------------------------------------------------------ */
static void append_val(seq_t v)
{
    if (vals_size == vals_cap) {
        size_t new_cap = vals_cap ? vals_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(vals, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);           /* allocation failure */
        vals = tmp;
        vals_cap = new_cap;
    }
    vals[vals_size++] = v;
    vals_sorted = 0;                /* unsorted after insertion */
}

/* ------------------------------------------------------------------
 * Comparator for qsort to order seq_t values.
 * ------------------------------------------------------------------ */

/* ------------------------------------------------------------------
 * Generate all values for r in (old_max, new_max] and store them.
 * ------------------------------------------------------------------ */
static void generate_to(idx_t new_max)
{
    for (idx_t r = max_r_processed + 1; r <= new_max; ++r) {
        for (idx_t s = r + 1; s < 2 * r; ++s) {
            if (gcd64((uint64_t)r, (uint64_t)s) != 1)
                continue;

            /* Compute s^2 - r^2 using 128‑bit arithmetic to avoid overflow. */
            __uint128_t val128 = (__uint128_t)s * s - (__uint128_t)r * r;
            seq_t val = (seq_t)val128;
            append_val(val);
        }
    }
    max_r_processed = new_max;
}

/* ------------------------------------------------------------------
 * Ensure that at least n distinct sorted values are available.
 * ------------------------------------------------------------------ */
static void ensure_n(idx_t n)
{
    /* Batch size for increasing r. Adjust as needed for performance. */
    const idx_t batch = 200;

    while (1) {
        /* If we already have enough sorted values, we are done. */
        if (vals_sorted && vals_size >= (size_t)n)
            break;

        /* Generate more values in batches of r. */
        generate_to(max_r_processed + batch);

        /* Sort and deduplicate the entire array. */
        if (!vals_sorted) {
            if (vals_size > 0) {
                qsort(vals, vals_size, sizeof(seq_t), cmp_seq);
                /* Deduplicate in place. */
                size_t uniq = 1;
                for (size_t i = 1; i < vals_size; ++i) {
                    if (vals[i] != vals[uniq - 1]) {
                        vals[uniq++] = vals[i];
                    }
                }
                vals_size = uniq;
            }
            vals_sorted = 1;
        }
        /* Loop again if still not enough values. */
    }
}

/* ------------------------------------------------------------------
 * Public sequence function with 1‑based indexing.
 * ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                  /* undefined for n=0 */

    ensure_n(n);                   /* produce at least n values */
    return vals[n - 1];             /* 1‑based index */
}
