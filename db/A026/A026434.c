/*
 * A026434  a(n) = least positive integer > a(n-1) and not a(i)*a(j)-1 for
 *          1<=i<j<=n, with initial terms 1,4.
 * https://oeis.org/A026434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 148, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A026434: a(1)=1, a(2)=4;  
   a(n) = least integer > a(n-1) not equal to a(i)*a(j)-1 for 1≤i<j<n */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

static seq_t *seq = NULL;   /* array holding computed terms */
static idx_t seq_len = 0;   /* number of terms stored */
static idx_t seq_cap = 0;   /* current allocated capacity */

/* binary search for target in seq[start..len-1] */
static bool contains(const seq_t *arr, idx_t start, idx_t len, seq_t target)
{
    idx_t lo = start;
    idx_t hi = len - 1;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        seq_t val = arr[mid];
        if (val < target) {
            lo = mid + 1;
        } else if (val > target) {
            if (mid == 0) break;          /* avoid underflow */
            hi = mid - 1;
        } else {
            return true;
        }
    }
    return false;
}

/* check if y equals a(i)*a(j) for some 0≤i<j< len */
static bool is_forbidden(seq_t y, const seq_t *arr, idx_t len)
{
    for (idx_t i = 0; i < len; ++i) {
        seq_t ai = arr[i];
        if (y % ai == (seq_t)0) {
            seq_t b = y / ai;
            if (contains(arr, i + 1, len, b))
                return true;
        }
    }
    return false;
}

/* ensure array capacity reaches at least needed */
static void ensure_capacity(idx_t needed)
{
    if (needed <= seq_cap) return;
    idx_t new_cap = seq_cap ? seq_cap * 2 : 2;
    if (new_cap < needed) new_cap = needed;
    seq_t *new_arr = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!new_arr) exit(EXIT_FAILURE);
    seq = new_arr;
    seq_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* undefined but safe */

    /* initialise first two terms */
    if (seq_len < 2) {
        ensure_capacity(2);
        seq[0] = (seq_t)1;
        seq[1] = (seq_t)4;
        seq_len = 2;
    }

    /* compute terms up to n */
    for (idx_t cur = seq_len; cur < n; ++cur) {
        seq_t candidate = seq[seq_len - 1] + (seq_t)1;
        for (;;) {
            seq_t y = candidate + (seq_t)1;   /* check product equals y */
            if (!is_forbidden(y, seq, seq_len)) {
                ensure_capacity(seq_len + 1);
                seq[seq_len++] = candidate;
                break;
            }
            ++candidate;
        }
    }
    return seq[n - 1];
}
