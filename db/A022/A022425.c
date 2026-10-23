/*
 * A022425  Solution a( ) of the complementary equation a(n) = b(n-1) +
 *          b(n-2), where a(0) = 1, a(1) = 4; see Comments.
 * https://oeis.org/A022425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A022425
 *  a(0) = 1, a(1) = 4;
 *  a(n) = b(n-1) + b(n-2)   for n ≥ 2;
 *  b(0) = least positive integer not in {a(0), a(1)};
 *  b(n) = least positive integer not in {a(0)…a(n), b(0)…b(n-1)}   for n > 0.
 *
 *  The code builds a(n) and its complementary sequence b(n) on demand,
 *  keeping the set of all previously used integers in an array.  The
 *  "next missing" integer is advanced by linear search until it is
 *  not present in the used array.  This works for any seq_t that
 *  supports the usual arithmetic and comparison operators, including
 *  arbitrary‑precision integer classes.  The implementation is valid
 *  C11 and C++17.  No external utilities are required. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- static storage ------------------------------------------ */
static idx_t      a_len  = 0;   /* number of a( ) values currently stored   */
static seq_t      *a_arr = NULL;
static size_t     a_cap  = 0;

static seq_t      *b_arr = NULL;
static size_t     b_cap  = 0;

static seq_t      *used_arr = NULL;  /* array of all used integers          */
static size_t     used_cnt = 0;
static size_t     used_cap = 0;

static seq_t      next_missing = (seq_t)1;   /* smallest candidate not used yet  */

/* ---------- helper routines ------------------------------------------ */

/* enlarge a_arr to be able to store at least (index+1) values   */
static void ensure_a_cap(idx_t index)
{
    while (a_cap <= (size_t)index) {
        size_t new_cap = a_cap ? a_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        a_arr = tmp;
        a_cap = new_cap;
    }
}

/* enlarge b_arr to be able to store at least (index+1) values   */
static void ensure_b_cap(idx_t index)
{
    while (b_cap <= (size_t)index) {
        size_t new_cap = b_cap ? b_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(b_arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        b_arr = tmp;
        b_cap = new_cap;
    }
}

/* enlarge used_arr to be able to store at least (needed) values   */
static void ensure_used_cap(idx_t needed)
{
    while (used_cap < (size_t)needed) {
        size_t new_cap = used_cap ? used_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(used_arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        used_arr = tmp;
        used_cap = new_cap;
    }
}

/* test whether a value has already been used (linear search)   */
static bool used_contains(seq_t val)
{
    for (size_t i = 0; i < used_cnt; ++i)
        if (used_arr[i] == val) return true;
    return false;
}

/* insert a value into the used table   */
static void used_add(seq_t val)
{
    ensure_used_cap(used_cnt + 1);
    used_arr[used_cnt++] = val;
}

/* advance next_missing until it points to an unused integer   */
static void advance_next_missing(void)
{
    while (used_contains(next_missing)) ++next_missing;
}

/* ---------- main sequence function ---------------------------------- */

seq_t a(idx_t n)
{
    /* initialise the sequence on the first call */
    if (a_len == 0) {
        /* capacities for the two sequences */
        ensure_a_cap(1);
        ensure_b_cap(1);

        /* capacity for used values */
        ensure_used_cap(4);

        /* a(0) = 1 */
        a_arr[0] = (seq_t)1;
        used_add(a_arr[0]);

        /* b(0) = 2  (smallest positive integer not in {1,4}) */
        b_arr[0] = (seq_t)2;
        used_add(b_arr[0]);

        /* a(1) = 4 */
        a_arr[1] = (seq_t)4;
        used_add(a_arr[1]);

        /* b(1) = 3  (smallest positive integer not in {1,4,2}) */
        b_arr[1] = (seq_t)3;
        used_add(b_arr[1]);

        a_len = 2;   /* a(0) and a(1) are ready   */
    }

    /* generate terms up to index n on demand */
    while (a_len <= n) {
        idx_t i = a_len;          /* next index to compute */

        /* ensure storage exists for this index */
        ensure_a_cap(i);
        ensure_b_cap(i);

        /* a(i) = b(i-1) + b(i-2)   (valid for i >= 2) */
        seq_t ai = b_arr[i - 1] + b_arr[i - 2];
        a_arr[i] = ai;
        used_add(ai);
        advance_next_missing();

        /* b(i) = current smallest unused integer */
        seq_t bi = next_missing;
        used_add(bi);               /* mark it as used too      */
        advance_next_missing();
        b_arr[i] = bi;

        ++a_len;   /* one more a() value available   */
    }

    return a_arr[n];
}
