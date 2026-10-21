/*
 * A075652  Triangle read by rows in which the n-th row is an arithmetic
 *          progression of n terms with a common difference of n, and the
 *          first term is chosen the lowest possible so that no number appears
 *          twice.
 * https://oeis.org/A075652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is built from the triangle described in the OEIS entry.
 *            Each row r is an arithmetic progression of r terms with common
 *            difference r, and the first term is the smallest positive
 *            integer that keeps all terms distinct. The implementation builds
 *            rows on demand and keeps a dynamic bitset of used...
 * Range:     u8 n <= 90, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   row_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/row_of.h"

/* Dynamic bitset for used integers (1‑based indexing). */
static uint64_t *used_bits = NULL;
static size_t used_words = 0;   /* number of 64‑bit words */

/* Grow the bitset to accommodate bits up to v. */
static void ensure_used_bits(idx_t v)
{
    /* Number of words needed for bits up to v. */
    size_t needed = (v + 63) >> 6;
    if (needed <= used_words)
        return;

    size_t new_words = used_words ? (used_words << 1) : 64;
    if (new_words < needed)
        new_words = needed;

    uint64_t *new_bits = (uint64_t *)realloc(used_bits, new_words * sizeof(uint64_t));
    if (!new_bits)
        abort();
    if (used_bits) {
        /* Zero the newly added words. */
        memset(new_bits + used_words, 0,
               (new_words - used_words) * sizeof(uint64_t));
    } else {
        /* First allocation. */
        memset(new_bits, 0, new_words * sizeof(uint64_t));
    }
    used_bits = new_bits;
    used_words = new_words;
}

static inline int is_used(idx_t v)
{
    return (used_bits[(v - 1) >> 6] >> ((v - 1) & 63)) & 1ULL;
}

static inline void set_used(idx_t v)
{
    used_bits[(v - 1) >> 6] |= (1ULL << ((v - 1) & 63));
}

/* Row management: starting value of each computed row. */
static seq_t *row_start = NULL;   /* 1‑based rows stored as 0‑based array */
static size_t rows_cap   = 0;
static size_t rows_size  = 0;     /* number of rows computed so far */

static idx_t min_unused = 1;

/* Compute rows up to 'target_row' (inclusive). */
static void ensure_rows(idx_t target_row)
{
    while (rows_size < target_row) {
        idx_t r = rows_size + 1;          /* current row to compute */
        seq_t start = (seq_t)min_unused;  /* candidate start */

        /* Search for the minimal start that yields all unused terms. */
        for (;;) {
            idx_t cand   = (idx_t)start;
            idx_t last   = cand + (r - 1) * r;   /* highest number in progression */
            ensure_used_bits(last);

            int ok = 1;
            for (idx_t k = 0; k < r; ++k) {
                idx_t v = cand + k * r;
                if (is_used(v)) { ok = 0; break; }
            }
            if (ok) break;   /* found suitable start */
            ++start;         /* try next candidate */
        }

        /* Mark the numbers of the new row as used. */
        idx_t start_idx = (idx_t)start;
        idx_t last = start_idx + (r - 1) * r;
        ensure_used_bits(last);
        for (idx_t k = 0; k < r; ++k)
            set_used(start_idx + k * r);

        /* Update minimum unused integer. */
        while (is_used(min_unused)) ++min_unused;

        /* Store the row start. */
        if (rows_size == rows_cap) {
            size_t new_cap = rows_cap ? (rows_cap << 1) : 16;
            seq_t *new_arr = (seq_t *)realloc(row_start, new_cap * sizeof(seq_t));
            if (!new_arr)
                abort();
            row_start = new_arr;
            rows_cap = new_cap;
        }
        row_start[rows_size++] = start;
    }
}

/* Public interface: compute a(n). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* guard for n < 1 */

    idx_t r = row_of(n);         /* row that contains term n */
    ensure_rows(r);              /* ensure rows up to r are computed */

    /* index of n within its row (0‑based) */
    idx_t prev_tri = (r - 1) * r / 2;      /* T(r‑1) */
    idx_t j = n - prev_tri - 1;            /* 0 ≤ j < r */

    seq_t start = row_start[r - 1];        /* starting value of row r */
    seq_t term  = start + j * (seq_t)r;    /* actual n‑th term */

    return term;
}
