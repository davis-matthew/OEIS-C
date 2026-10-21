/*
 * A081994  Triangle read by rows in which the n-th row contains n numbers not
 *          occurring earlier such that every entry is coprime to all the
 *          members of the row and column it belongs.
 * https://oeis.org/A081994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, nonn
 * Formula:   a(n) is obtained by constructing the triangle described in
 *            A081994: * each row contains new numbers, none occurring before,
 *            each being coprime to * all numbers in its row and its column.
 *            This program builds the triangle * incrementally and memoises
 *            the results up to the largest n requested
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* Dynamic arrays for the sequence and the set of used numbers */
static seq_t *seq = NULL;          /* sequence values in row‑major order    */
static size_t seq_cap = 0;          /* capacity of seq array                */
static idx_t  seq_len = 0;          /* number of values currently stored    */

static seq_t *used = NULL;         /* values that have already appeared    */
static size_t used_cap = 0;
static size_t used_cnt = 0;

/* Number of rows already constructed (rows are counted starting from 1) */
static idx_t rows_filled = 0;

/* Ensure the sequence array has at least 'need' slots.  Returns nothing. */
static void ensure_seq_cap(size_t need)
{
    if (seq_cap >= need) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 64;
    while (new_cap < need) new_cap <<= 1;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Ensure the used array can hold 'need' elements. */
static void ensure_used_cap(size_t need)
{
    if (used_cap >= need) return;
    size_t new_cap = used_cap ? used_cap * 2 : 64;
    while (new_cap < need) new_cap <<= 1;
    used = (seq_t *)realloc(used, new_cap * sizeof(seq_t));
    used_cap = new_cap;
}

/* Linear search whether val has appeared before. */
static bool is_used(seq_t val)
{
    for (size_t i = 0; i < used_cnt; ++i)
        if (used[i] == val) return true;
    return false;
}

/* Record a newly used value. */
static void add_used(seq_t val)
{
    ensure_used_cap(used_cnt + 1);
    used[used_cnt++] = val;
}

/* Find the smallest integer for position (row r, column c).  Tests for
 * uniqueness and coprimality with all earlier numbers in the same row
 * and column. */
static seq_t find_candidate(idx_t r, idx_t c)
{
    seq_t candidate = 1;
    for (;; ++candidate)
    {
        if (!is_used(candidate))
        {
            bool ok = true;

            /* Check previously placed numbers in the same row (to the left). */
            idx_t row_offset = (r - 1) * r / 2;
            for (idx_t j = 1; j < c; ++j)
            {
                seq_t other = seq[row_offset + (j - 1)];
                if (gcd(candidate, other) != (seq_t)1) { ok = false; break; }
            }

            /* Check numbers above in the same column. */
            if (ok)
            {
                for (idx_t i = c; i < r; ++i)
                {
                    idx_t col_offset = (i - 1) * i / 2;
                    seq_t other = seq[col_offset + (c - 1)];
                    if (gcd(candidate, other) != (seq_t)1) { ok = false; break; }
                }
            }

            if (ok) return candidate;
        }
    }
}

/* Build the triangle up to at least 'rows' rows.  Only new rows are added. */
static void build_rows(idx_t rows)
{
    while (rows_filled < rows)
    {
        ++rows_filled;
        for (idx_t c = 1; c <= rows_filled; ++c)
        {
            seq_t val = find_candidate(rows_filled, c);
            ensure_seq_cap(seq_len + 1);
            seq[seq_len++] = val;
            add_used(val);
        }
    }
}

/* Main interface required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* Offset is 1; safeguard against n=0. */

    /* Determine how many rows are needed to cover index n. */
    idx_t need_rows = 0;
    while (need_rows * (need_rows + 1) / 2 < n) ++need_rows;

    /* Grow the triangle as necessary. */
    build_rows(need_rows);

    /* Sequence is stored zero‑based; index n-1 gives a(n). */
    return seq[n - 1];
}
