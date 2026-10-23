/*
 * A164738  Triangle read by rows. Row 0 = {2}; left half of row n+1 = row n,
 *          right half = row n reversed with each term replaced by the next
 *          prime.
 * https://oeis.org/A164738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is obtained by concatenating rows of a triangle. Row[0] =
 *            {2} Row[r+1] = Row[r] ∘ (next_prime_of(reversed(Row[r]))) a(n)
 *            is the n‑th element of this concatenation (0‑based)
 * Range:     u8 n <= 9076, u16 n <= 9076, u32 n <= 9076, u64 n <= 9076,
 *            u128 n <= 9076, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------------ */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* ------------------------------------------------------------------ */
/* find the next prime after v (v fits in seq_t but may be larger after promotion) */
static seq_t next_prime_of(seq_t v)
{
    /* promote to a type that can hold larger values during the search */
    uint64_t cand = (uint64_t)(v) + 1;
    while (!is_prime((idx_t)cand))
        ++cand;
    return (seq_t)cand;          /* cast back to seq_t (possible wrap‑around) */
}

/* ------------------------------------------------------------------ */
/* static storage for generated rows */
#define MAX_ROWS 64
static seq_t *rows[MAX_ROWS];
static size_t row_len[MAX_ROWS];
static int max_row_computed = -1;   /* -1 means no rows computed yet */

/* ensure that Row[r] (with all rows up to r) has been built */
static void ensure_row(unsigned int r)
{
    for (int i = max_row_computed + 1; i <= (int)r; ++i) {
        if (i == 0) {
            /* Row 0 = {2} */
            rows[0] = (seq_t *)calloc(1, sizeof(seq_t));
            rows[0][0] = (seq_t)2;
            row_len[0] = 1;
        } else {
            size_t prev_len = row_len[i - 1];
            size_t curr_len = 1ULL << i;          /* 2^i */
            seq_t *row = (seq_t *)calloc(curr_len, sizeof(seq_t));
            /* left half – copy previous row */
            for (size_t k = 0; k < prev_len; ++k)
                row[k] = rows[i - 1][k];
            /* right half – reversed previous row, next primes */
            for (size_t k = 0; k < prev_len; ++k) {
                seq_t v = rows[i - 1][prev_len - 1 - k];
                row[prev_len + k] = next_prime_of(v);
            }
            rows[i] = row;
            row_len[i] = curr_len;
        }
        max_row_computed = i;
    }
}

/* ------------------------------------------------------------------ */
/* public sequence function */
seq_t a(idx_t n)
{
    /* Find row r that contains the index n.
       Row r has 2^r <= (n+1) < 2^(r+1). */
    idx_t m = n + 1;            /* 1‑based position */
    unsigned int r = 0;
    idx_t pow = 1;
    while ((pow << 1) <= m) {
        pow <<= 1;
        ++r;
    }
    /* position within the row (0‑based) */
    idx_t p = m - pow;
    /* Build rows up to r if necessary */
    ensure_row(r);
    /* fetch the element */
    return rows[r][(size_t)p];
}
