/*
 * A096107  Triangle read by rows: row n lists cubic residues modulo n.
 * https://oeis.org/A096107
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = n‑th term of the triangle read by rows where row k lists
 *            the distinct cubic residues modulo k obtained from integers a
 *            with gcd(a,k)=1. The sequence has offset 2 (a(2) is the first
 *            term)
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* auxiliary gcd for idx_t (uint64_t) */
static inline idx_t gcd_idx(idx_t a, idx_t b) {
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* cumulative counts of terms up to each row */
static idx_t *cum_counts = NULL;
static size_t   cum_cap    = 0;
static idx_t    max_row    = 1;   /* last processed row */

/* bit array to mark residues within a row */
static unsigned char *row_seen = NULL;
static size_t         seen_cap = 0;

/* build cumulative counts up to a target position (zero‑based) */
static void ensure_cumulative(idx_t target_pos) {
    if (!cum_counts) { /* initialisation */
        cum_cap   = 4;    /* indices 0..3 */
        cum_counts = (idx_t *)calloc(cum_cap, sizeof(idx_t));
        if (!cum_counts) return; /* allocation failure, will fail gracefully */
        cum_counts[0] = 0;
        cum_counts[1] = 0;      /* row 1 unused */
        max_row = 1;
    }

    /* extend the cumulative array until it covers target_pos */
    while (cum_counts[max_row] <= target_pos) {
        idx_t next_row = max_row + 1;

        /* allocate enough space for residue bitset for this row */
        if (seen_cap < next_row) {
            unsigned char *tmp = (unsigned char *)realloc(row_seen, next_row);
            if (!tmp) return; /* allocation failure */
            row_seen = tmp;
            seen_cap = next_row;
        }
        memset(row_seen, 0, next_row);

        /* count distinct residues for this row */
        size_t distinct = 0;
        for (idx_t a = 1; a < next_row; ++a) {
            if (gcd_idx(a, next_row) != 1) continue;
            seq_t r = (seq_t)a * (seq_t)a % (seq_t)next_row;
            r = r * (seq_t)a % (seq_t)next_row;
            if (r == 0) continue;                     /* should not happen for coprime a */
            idx_t ri = (idx_t)r;
            if (!row_seen[ri]) {
                row_seen[ri] = 1;
                ++distinct;
            }
        }

        /* enlarge cum_counts array if necessary */
        if (cum_cap <= next_row) {
            size_t new_cap = cum_cap * 2 + 10;
            idx_t *tmp = (idx_t *)realloc(cum_counts, new_cap * sizeof(idx_t));
            if (!tmp) return; /* allocation failure */
            cum_counts = tmp;
            cum_cap = new_cap;
        }
        cum_counts[next_row] = cum_counts[max_row] + distinct;
        max_row = next_row;
    }
}

/* return the n-th term of the flattened triangle (offset 2) */
seq_t a(idx_t n) {
    if (n < 2) return 0;   /* undefined for n < offset */

    idx_t target_pos = n - 2;   /* zero‑based position in the flattened list */
    ensure_cumulative(target_pos);
    if (!cum_counts) return 0;  /* allocation failed */

    /* binary search to locate the row containing target_pos */
    idx_t lo = 2, hi = max_row, row = max_row;
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (cum_counts[mid] > target_pos) {
            row = mid;
            if (mid == 0) break;
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }

    idx_t prev_cum = cum_counts[row - 1];
    idx_t pos_in_row = target_pos - prev_cum + 1;   /* 1‑based within this row */

    /* rebuild residue bitset for the identified row */
    if (seen_cap < row) {
        unsigned char *tmp = (unsigned char *)realloc(row_seen, row);
        if (!tmp) return 0;
        row_seen = tmp;
        seen_cap = row;
    }
    memset(row_seen, 0, row);

    for (idx_t a_val = 1; a_val < row; ++a_val) {
        if (gcd_idx(a_val, row) != 1) continue;
        seq_t r = (seq_t)a_val * (seq_t)a_val % (seq_t)row;
        r = r * (seq_t)a_val % (seq_t)row;
        if (r == 0) continue;
        idx_t ri = (idx_t)r;
        row_seen[ri] = 1;
    }

    /* scan residues in ascending order to find the desired element */
    for (idx_t val = 1; val < row; ++val) {
        if (row_seen[val]) {
            if (pos_in_row == 1) return (seq_t)val;
            --pos_in_row;
        }
    }

    return 0;   /* should never be reached */
}
