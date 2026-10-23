/*
 * A020670  Numbers of form x^2 + 7y^2.
 * https://oeis.org/A020670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A020670: Numbers of the form x^2 + 7y^2, sorted in increasing
 * order with duplicates removed. */

#include <stdlib.h>
#include <stdbool.h>

/* Maximum number of terms we will store in the static tables. Adjust
   if larger sequences are needed; 200 000 covers thousands of terms
   comfortably for any of the supported seq_t sizes. */
#define MAX_CAP 200000

/* Static buffers for the sorted unique values and a temporary array
   used during regeneration. */
static seq_t values[MAX_CAP];
static size_t values_len = 0;

static seq_t tmp_vals[MAX_CAP];
static size_t tmp_len = 0;

/* Current maximum value that has been generated.  When the list of
 * terms is too short, this value will be doubled and the tables
 * regenerated. */
static idx_t current_max_val = 0;

/* Comparator for qsort that works for any seq_t implementation. */
static int cmp_seq(const void *p1, const void *p2)
{
    const seq_t *v1 = (const seq_t *)p1;
    const seq_t *v2 = (const seq_t *)p2;
    if (*v1 < *v2) return -1;
    if (*v1 > *v2) return 1;
    return 0;
}

/* Rebuild the entire list of representable numbers up to <= max_val.
 * All previous data is discarded. */
static void rebuild(idx_t max_val)
{
    /* Compute the largest possible y such that 7*y^2 <= max_val. */
    idx_t y_max = 0;
    while ((seq_t)7 * (seq_t)(y_max + 1) * (seq_t)(y_max + 1) <= max_val)
        ++y_max;

    tmp_len = 0;

    for (idx_t y = 0; y <= y_max; ++y) {
        seq_t base = (seq_t)7 * (seq_t)y * (seq_t)y;
        idx_t x = 0;
        while (1) {
            seq_t val = base + (seq_t)x * (seq_t)x;
            if (val > max_val)
                break;
            if (tmp_len < MAX_CAP)
                tmp_vals[tmp_len++] = val;
            ++x;
        }
    }

    /* Sort and deduplicate into the values table. */
    qsort(tmp_vals, tmp_len, sizeof(seq_t), cmp_seq);

    values_len = 0;
    for (size_t i = 0; i < tmp_len; ++i) {
        if (i == 0 || tmp_vals[i] != tmp_vals[i - 1]) {
            if (values_len < MAX_CAP)
                values[values_len++] = tmp_vals[i];
        }
    }
}

/* Ensure that we have at least n terms available.  If not, we
 * double the current_max_val until enough terms are generated. */
static void ensure_terms(idx_t n)
{
    if (values_len >= (size_t)n)
        return;
    if (current_max_val == 0)
        current_max_val = 1000;                 /* arbitrary start */

    while (1) {
        rebuild(current_max_val);
        if (values_len >= (size_t)n)
            return;
        /* Avoid overflow when doubling. */
        if (current_max_val > ((idx_t)1 << 62))
            break;
        current_max_val *= 2;
    }
}

/* Public function required by the interface. */
seq_t a(idx_t n)
{
    if (n == 0)               /* sequence offset is 1, undefined for 0 */
        return 0;
    ensure_terms(n);
    return values[(size_t)(n - 1)];
}
