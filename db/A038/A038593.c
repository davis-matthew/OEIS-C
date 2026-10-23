/*
 * A038593  Differences between positive cubes in 1, 2 or 3 ways: union of
 *          A014439, A014440 and A014441.
 * https://oeis.org/A038593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Union of all differences of positive cubes that have exactly 1,
 *            2 or 3 representations. The function generates all differences
 *            a^3 - b^3 (a>b>0), counts how many times each difference occurs,
 *            keeps those with at most 3 occurences, sorts them and returns
 *            the n‑th term
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Compare function for qsort */
static int compare_seq_t(const void *v1, const void *v2)
{
    const seq_t *a = (const seq_t *)v1;
    const seq_t *b = (const seq_t *)v2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* Global storage for the precomputed sequence */
static seq_t *valids = NULL;          /* sorted list of valid differences */
static size_t val_count = 0;          /* number of valid differences */
static bool initialized = false;      /* flag if sequence built */
static idx_t current_A_MAX = 0;       /* current limit for 'a' in a^3 */

/* Build the list for a given limit of 'a'.  All work is done
   in this function; old data are freed. */
static void build_list(idx_t a_max)
{
    /* Total number of unordered pairs (a,b) with 1≤b<a≤a_max */
    size_t total_pairs = (size_t)a_max * (a_max - 1) / 2;
    seq_t *diffs = NULL;
    if (diffs) { free(diffs); }
    diffs = (seq_t *)malloc(total_pairs * sizeof(seq_t));

    /* Generate all differences a^3 - b^3 */
    size_t idx = 0;
    for (idx_t a = 2; a <= a_max; ++a) {
        seq_t a3 = (seq_t)a * a * a;
        for (idx_t b = 1; b < a; ++b) {
            seq_t d = a3 - (seq_t)b * b * b;
            diffs[idx++] = d;
        }
    }

    /* Sort the differences */
    qsort(diffs, total_pairs, sizeof(seq_t), compare_seq_t);

    /* Count duplicates and keep those occurring ≤3 times */
    /* Allocate maximum possible size first */
    seq_t *tmp_valids = (seq_t *)malloc(total_pairs * sizeof(seq_t));
    size_t v_idx = 0;
    for (size_t i = 0; i < total_pairs; ) {
        size_t j = i + 1;
        while (j < total_pairs && diffs[j] == diffs[i]) ++j;
        size_t count = j - i;
        if (count <= 3) {
            tmp_valids[v_idx++] = diffs[i];
        }
        i = j;
    }

    /* Free temporary structures */
    free(diffs);

    /* Replace old tables */
    if (valids) free(valids);
    valids = tmp_valids;
    val_count = v_idx;
}

/* Public function required by the build */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                   /* undefined for n==0 */

    /* Initial build with a reasonably large limit */
    if (!initialized) {
        current_A_MAX = 1200;               /* enough for n≤10000 */
        build_list(current_A_MAX);
        initialized = true;
    }

    /* If requested index exceeds current tables, enlarge */
    while (val_count < n) {
        current_A_MAX *= 2;                 /* double the limit */
        build_list(current_A_MAX);
    }

    return valids[n - 1];
}
