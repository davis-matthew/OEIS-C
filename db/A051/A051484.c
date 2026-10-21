/*
 * A051484  a(n) is the next natural number (besides 1) which is not congruent
 *          to a(i) mod a(j) for any i < j < n.
 * https://oeis.org/A051484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 267, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A051484: a(n) = smallest natural number (≠1) not congruent to a(i)
   modulo a(j) for any i < j < n.  The code builds the sequence
   incrementally and can answer any n ≥ 1, even out of order.
   It keeps a sorted table of values together with their first
   appearance index to test the congruence condition in O(log N)
   time for each j, with O(N^2 log N) total time for the first 10 000
   terms, well within the limits for 64‑bit builds.  It works with
   arbitrary‑precision seq_t as long as comparison operators
   (<, ==) and arithmetic (+, %) are defined.
*/

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* --- data structures --------------------------------------------------- */

typedef struct {
    seq_t value;   /* the sequence term   */
    idx_t index;   /* first index where value appears */
} pair_t;

/* sequence storage (1‑based) */
static idx_t seq_len = 0;        /* number of terms stored (≥0)      */
static size_t seq_cap = 0;       /* allocated capacity                 */
static seq_t *seq_arr = NULL;    /* array big enough for seq_cap items */

/* sorted table of (value, first_index) for fast membership tests */
static pair_t *first_index_arr = NULL;
static size_t first_index_cap = 0;
static size_t first_index_size = 0;

/* --- helper functions ---------------------------------------------------- */

/* ensure seq_arr can hold indices up to need (1‑based) */
static void ensure_seq_cap(size_t need)
{
    if (need + 1 > seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        if (new_cap < need + 1) new_cap = need + 1;
        seq_t *tmp = (seq_t *)realloc(seq_arr, sizeof(seq_t) * new_cap);
        if (!tmp) abort();
        seq_arr = tmp;
        seq_cap = new_cap;
    }
}

/* ensure first_index_arr can hold at least need elements */
static void ensure_first_index_cap(size_t need)
{
    if (need > first_index_cap) {
        size_t new_cap = first_index_cap ? first_index_cap * 2 : 16;
        if (new_cap < need) new_cap = need;
        pair_t *tmp = (pair_t *)realloc(first_index_arr,
                                        sizeof(pair_t) * new_cap);
        if (!tmp) abort();
        first_index_arr = tmp;
        first_index_cap = new_cap;
    }
}

/* binary search: returns position where value would be inserted
   (first ≥ value).                           */
static size_t binary_search_position(seq_t value)
{
    size_t low = 0, high = first_index_size;
    while (low < high) {
        size_t mid = (low + high) / 2;
        if (first_index_arr[mid].value < value)
            low = mid + 1;
        else
            high = mid;
    }
    return low;          /* may equal first_index_size */
}

/* return first index where val appears, or 0 if not present */
static idx_t find_first_index(seq_t val)
{
    size_t pos = binary_search_position(val);
    if (pos < first_index_size && first_index_arr[pos].value == val)
        return first_index_arr[pos].index;
    return 0;
}

/* insert val with its first appearance index if it is not present yet */
static void insert_first_index(seq_t val, idx_t index)
{
    size_t pos = binary_search_position(val);
    if (pos < first_index_size && first_index_arr[pos].value == val)
        return;                   /* already present */
    ensure_first_index_cap(first_index_size + 1);
    /* shift elements to make room */
    for (size_t i = first_index_size; i > pos; --i)
        first_index_arr[i] = first_index_arr[i - 1];
    first_index_arr[pos].value = val;
    first_index_arr[pos].index = index;
    first_index_size++;
}

/* --- compute the next sequence term ------------------------------------ */

static seq_t compute_next(idx_t current_index)
{
    /* current_index == seq_len, last stored term */
    seq_t candidate = seq_arr[current_index] + (seq_t)1;

    while (true) {
        bool ok = true;
        for (idx_t j = 2; j <= current_index; ++j) {
            seq_t rem = candidate % seq_arr[j];
            idx_t fi = find_first_index(rem);
            if (fi && fi < j) {
                ok = false;
                break;
            }
        }
        if (ok)
            return candidate;
        candidate = candidate + (seq_t)1;   /* try next integer */
    }
}

/* --- public function ----------------------------------------------------- */

seq_t a(idx_t n)
{
    /* Initialise on first call. */
    if (seq_len == 0) {
        ensure_seq_cap(16);
        seq_arr[1] = (seq_t)0;   /* a(1) = 0 (by definition) */
        seq_arr[2] = (seq_t)2;   /* a(2) = 2 */
        seq_len = 2;

        ensure_first_index_cap(16);
        insert_first_index(seq_arr[1], 1);   /* 0 first appears at 1 */
        insert_first_index(seq_arr[2], 2);   /* 2 first appears at 2 */
    }

    /* Generate terms up to n if necessary. */
    while (seq_len < n) {
        ensure_seq_cap(seq_len + 1);           /* need space for new term */
        seq_t next_val = compute_next(seq_len);
        ++seq_len;
        seq_arr[seq_len] = next_val;
        insert_first_index(next_val, seq_len);
    }

    return seq_arr[n];
}
