/*
 * A093681  Sequence contains no 3-term arithmetic progression, starting with
 *          1, 28.
 * https://oeis.org/A093681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A093681:  No 3-term arithmetic progression
   The sequence is computed by a greedy algorithm with the
   first two terms fixed to 1 and 28.
   a(n) = the least integer k > a(n-1) such that adding k does not form
   an arithmetic progression with any two earlier terms. */

/* Required types (provided by the build) */
#include <stdlib.h>
#include <stdbool.h>

/* Forward declarations for static helpers */
static void ensure_capacity(size_t needed);
static bool contains(const seq_t *arr, size_t len, seq_t val);

/* Static storage for the sequence */
static seq_t  *seq_data = NULL;
static size_t  seq_len  = 0;   /* number of elements currently stored */
static size_t  seq_cap  = 0;   /* allocated capacity */

/* Helper: binary search for a value in a sorted array */
static bool contains(const seq_t *arr, size_t len, seq_t val)
{
    size_t lo = 0, hi = len;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (arr[mid] < val)
            lo = mid + 1;
        else if (arr[mid] > val)
            hi = mid;
        else
            return true;
    }
    return false;
}

/* Grow the static buffer if necessary */
static void ensure_capacity(size_t needed)
{
    if (needed <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 8;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_arr = (seq_t *)realloc(seq_data, new_cap * sizeof(seq_t));
    if (!new_arr)
        abort(); /* allocation failed */
    seq_data = new_arr;
    seq_cap  = new_cap;
}

/* Main function required by the specification */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined for offset 1, but harmless */

    /* Generate terms up to n on demand */
    while (seq_len < (size_t)n) {
        ensure_capacity(seq_len + 1);

        /* Initial two terms are fixed */
        if (seq_len == 0) {
            seq_data[0] = (seq_t)1;
            seq_len++;
            continue;
        }
        if (seq_len == 1) {
            seq_data[1] = (seq_t)28;
            seq_len++;
            continue;
        }

        /* Greedy choice for the next term */
        seq_t candidate = seq_data[seq_len - 1] + (seq_t)1;

        for (;;) {
            bool bad = false;

            /* Test against all existing elements */
            for (size_t i = 0; i < seq_len; ++i) {
                seq_t x = seq_data[i];

                /* 1. Candidate as an extreme member of an AP:
                   t1 = 2*x - candidate   (must be positive and in the set) */
                seq_t t1;
                if (candidate <= x) {
                    /* candidate <= x  => 2*x - candidate = x + (x - candidate) */
                    t1 = x + (x - candidate);
                } else {
                    seq_t diff = candidate - x;          /* positive */
                    if (diff <= x)                      /* candidate <= 2*x */
                        t1 = x - diff;
                    else
                        continue;                       /* candidate > 2*x, skip */
                }
                if (t1 > (seq_t)0 && contains(seq_data, seq_len, t1)) {
                    bad = true;
                    break;
                }

                /* 2. Candidate as the middle member of an AP:
                   t2 = 2*candidate - x   (must be positive and in the set) */
                seq_t t2;
                if (x <= candidate) {
                    /* x <= candidate => 2*candidate - x = candidate + (candidate - x) */
                    t2 = candidate + (candidate - x);
                } else {
                    seq_t diff = x - candidate;          /* positive, diff < candidate? */
                    if (diff < candidate)                /* x < 2*candidate */
                        t2 = candidate - diff;
                    else
                        continue;                       /* x >= 2*candidate, skip */
                }
                if (t2 > (seq_t)0 && contains(seq_data, seq_len, t2)) {
                    bad = true;
                    break;
                }
            }

            if (!bad)
                break;          /* candidate accepted */
            ++candidate;       /* try next integer */
        }

        seq_data[seq_len] = candidate;
        ++seq_len;
    }

    return seq_data[n - 1];
}
