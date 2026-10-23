/*
 * A023775  Metadromes: digits in base 6 are in strict ascending order.
 * https://oeis.org/A023775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 32, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Metadromes: digits in base 6 are in strict ascending order.
   The sequence consists of all numbers whose base‑6 representation
   uses digits 1..5 in increasing order; zero itself is a term.
   This function enumerates those numbers, stores them in a
   static array on first call, and returns the n‑th term
   (offset 1).  For n beyond the finite sequence the result is 0. */
#include <stdlib.h>
#include <stdbool.h>

/* compare function for qsort */
static int seq_cmp(const void *p1, const void *p2)
{
    const seq_t a = *(const seq_t *)p1;
    const seq_t b = *(const seq_t *)p2;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

/* storage for the sequence; 32 terms in total */
static bool initialized = false;
static seq_t values[32];
static size_t value_count = 0;

/* generate the 32 terms and sort them */
static void init_values(void)
{
    seq_t tmp[32];
    size_t idx = 0;

    /* zero is the first term */
    tmp[idx++] = (seq_t)0;

    /* generate all non‑empty subsets of {1,2,3,4,5} */
    for (int mask = 1; mask < (1 << 5); ++mask) {
        seq_t val = (seq_t)0;
        for (int d = 1; d <= 5; ++d) {
            if (mask & (1 << (d - 1))) {
                val = val * (seq_t)6 + (seq_t)d;
            }
        }
        tmp[idx++] = val;
    }

    /* sort the generated values */
    qsort(tmp, idx, sizeof(seq_t), seq_cmp);

    /* copy into the static array */
    for (size_t i = 0; i < idx; ++i) {
        values[i] = tmp[i];
    }

    value_count = idx;
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) init_values();

    if (n < 1 || (size_t)n > value_count)
        return (seq_t)0;            /* undefined for indices beyond the finite sequence */

    return values[(size_t)n - 1];
}
