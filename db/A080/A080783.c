/*
 * A080783  a(1)=1, a(n)=a(n-1)-1 if n is already in the sequence,
 *          a(n)=a(n-1)+5 otherwise.
 * https://oeis.org/A080783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1 a(n) = a(n-1)-1 if n is already in the sequence,
 *            a(n-1)+5 otherwise
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence depends on all previous terms; the implementation keeps a
   static cache of computed terms and a sorted list of all seen values.
   For each new index we binary‑search the sorted list to decide whether
   the index is present, then compute the next term and insert it
   into the list.  This works for any integer type defined by the
   harness (uint8_t…uint64_t, unsigned __int128, or the arbitrary‑
   precision class).  */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *seq = NULL;           /* cache of sequence values               */
static size_t seq_cap = 0, seq_len = 0;
static seq_t *seen = NULL;          /* sorted list of seen sequence values    */
static size_t seen_cap = 0, seen_len = 0;
static bool initialized = false;

/* Ensure the caches are at least of the required size.     */
static void ensure_cache(size_t n) {
    if (!initialized) {
        seq_cap = 64;
        seq_len = 0;
        seq = (seq_t *)malloc(seq_cap * sizeof *seq);

        seen_cap = 32;
        seen_len = 0;
        seen = (seq_t *)malloc(seen_cap * sizeof *seen);

        /* first term a(1) = 1 */
        seq[seq_len++] = (seq_t)1;
        seen[seen_len++] = (seq_t)1;
        initialized = true;
    }
    if (seq_len < n) {
        size_t new_cap = seq_cap;
        while (new_cap < n) new_cap <<= 1;
        seq = (seq_t *)realloc(seq, new_cap * sizeof *seq);
        seq_cap = new_cap;
    }
}

/* Binary search: true if v is in seen[0..seen_len-1]      */
static bool seen_contains(seq_t v) {
    size_t lo = 0, hi = seen_len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (seen[mid] < v) lo = mid + 1;
        else if (seen[mid] > v) hi = mid;
        else return true;
    }
    return false;
}

/* Insert v into seen keeping the array sorted.            */
static void seen_insert(seq_t v) {
    size_t lo = 0, hi = seen_len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (seen[mid] < v) lo = mid + 1;
        else hi = mid;
    }
    /* lo is insertion point */
    if (seen_len == seen_cap) {
        size_t new_cap = seen_cap << 1;
        seen = (seq_t *)realloc(seen, new_cap * sizeof *seen);
        seen_cap = new_cap;
    }
    for (size_t i = seen_len; i > lo; --i) seen[i] = seen[i-1];
    seen[lo] = v;
    ++seen_len;
}

/* Main function required by the harness. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* undefined for n<1, return 0 */

    ensure_cache((size_t)n);
    for (size_t cur = seq_len; cur < (size_t)n; ++cur) {
        seq_t prev = seq[seq_len-1];
        seq_t next;
        if (seen_contains((seq_t)(cur+1)))   /* cur is 0‑based, index n=cur+1 */
            next = prev - 1;
        else
            next = prev + 5;
        seq[seq_len++] = next;
        seen_insert(next);
    }
    return seq[(size_t)n - 1];
}
