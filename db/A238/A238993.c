/*
 * A238993  Number of nX4 0..3 arrays with no element equal to two plus the
 *          sum of elements to its left or two plus the sum of the sum of
 *          elements above it, modulo 4.
 * https://oeis.org/A238993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A238993
 *   Number of n×4 arrays over {0,1,2,3} with no element equal to
 *   (2 + sum of elements to its left) mod 4 or (2 + sum of elements
 *   above it) mod 4.
 *   Computation: dynamic programming over the column‑sum residues
 *   modulo 4 after each row.  There are 4^2 = 256 states; the
 *   transition from a state to another is obtained by enumerating all
 *   4‑tuples of row entries satisfying the constraints.  The total
 *   number of arrays of size n is the sum of the DP vector after the
 *   nth row.  The algorithm runs in O(n·256²) ≈ O(n) time and uses
 *   only 256·256·2 bytes of constant memory (512 KB for transition
 *   table).
 *
 * The function a(idx_t n) returns this count for any n ≥ 1, using
 * memoisation so that calls in arbitrary order remain O(n) for the
 * largest n seen.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define STATES 256     /* 4 columns, each 2 bits (0..3) */

static uint16_t trans[STATES][STATES];
static bool trans_initialized = false;

/* ------------------------------------------------------------ */
/*   encode / decode the column‑sum residue state (4 2‑bit fields) */
static inline int encode_state(const int col[4])
{
    /* bit layout: col[0] in bits 0‑1, col[1] in 2‑3, col[2] in 4‑5, col[3] in 6‑7 */
    return (col[0]) | (col[1] << 2) | (col[2] << 4) | (col[3] << 6);
}

static inline void decode_state(int s, int col[4])
{
    for (int j = 0; j < 4; ++j) {
        col[j] = s & 3;
        s >>= 2;
    }
}

/* ------------------------------------------------------------ */
/*   Build transition table:  trans[s][t] = #rows mapping state s → t */
static void init_trans(void)
{
    if (trans_initialized) return;

    for (int s = 0; s < STATES; ++s) {
        int col[4];
        decode_state(s, col);

        /* initialise row counts for this source state */
        for (int t = 0; t < STATES; ++t) trans[s][t] = 0;

        /* enumerate all 4‑tuples of row entries (0..3) */
        for (int r = 0; r < STATES; ++r) {
            int newcol[4];
            int left = 0;                /* running sum of current row */
            bool ok   = true;
            int rr = r;
            for (int j = 0; j < 4; ++j) {
                int val  = rr & 3;
                int forb1 = (left + 2) & 3;          /* left constraint */
                int forb2 = (col[j] + 2) & 3;        /* upper constraint */
                if (val == forb1 || val == forb2) { ok = false; break; }

                left      = (left + val) & 3;
                newcol[j] = (col[j] + val) & 3;
                rr >>= 2;
            }
            if (!ok) continue;

            int dest = encode_state(newcol);
            trans[s][dest]++;               /* one way */
        }
    }
    trans_initialized = true;
}

/* ------------------------------------------------------------ */
/*   DP arrays for the current row distribution                */
static seq_t  counts[STATES];
static seq_t  next_counts[STATES];
static bool counts_initialized = false;

/* ------------------------------------------------------------ */
/*   Cached sequence values                                    */
static seq_t *cached_results   = NULL;
static size_t cached_len       = 0;   /* number of terms already computed */
static size_t cached_cap       = 0;   /* allocated capacity */

/* ------------------------------------------------------------ */
/*   Main function                                               */
seq_t a(idx_t n)
{
    /* initialise transition table if first call */
    if (!trans_initialized) init_trans();

    /* initialise DP counts for the first row (col sums all zero) */
    if (!counts_initialized) {
        memset(counts, 0, sizeof(counts));
        counts[0] = 1;                   /* start from state 0 (all zeros) */
        counts_initialized = true;
    }

    /* extend cached results up to n if necessary */
    if (cached_len < n) {
        /* grow cached_results buffer */
        if (n > cached_cap) {
            size_t new_cap = cached_cap ? cached_cap << 1 : 1;
            if (new_cap < n) new_cap = n;
            seq_t *tmp = (seq_t*)realloc(cached_results,
                                         new_cap * sizeof(seq_t));
            if (!tmp) return 0;          /* allocation failure → zero term */
            cached_results = tmp;
            cached_cap     = new_cap;
        }

        /* compute rows from cached_len+1 up to n */
        for (idx_t row = cached_len + 1; row <= n; ++row) {
            memset(next_counts, 0, sizeof(next_counts));

            for (int s = 0; s < STATES; ++s) {
                if (counts[s] == 0) continue;
                seq_t cs = counts[s];
                for (int t = 0; t < STATES; ++t) {
                    uint16_t ways = trans[s][t];
                    if (!ways) continue;
                    next_counts[t] += cs * (seq_t)ways;
                }
            }

            /* swap counts */
            memcpy(counts, next_counts, sizeof(counts));

            /* sum over all states to obtain a(row) */
            seq_t sum = 0;
            for (int t = 0; t < STATES; ++t) sum += counts[t];
            cached_results[row - 1] = sum;
        }
        cached_len = n;
    }

    return cached_results[n - 1];
}
