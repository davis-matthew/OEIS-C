/*
 * A026435  a(n) = least positive integer > a(n-1) and not a(i)*a(j)-1 for
 *          1<=i<j<=n, with initial terms 2,4.
 * https://oeis.org/A026435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 154, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = least integer > a(n-1) that is not of the form
 * a(i)*a(j)-1 for any 1 ≤ i < j < n.  The two known values are
 * a(1)=2, a(2)=4.  The function memoises previously computed terms. */

#include <stdlib.h>
#include <stdbool.h>

/* static memoisation storage */
static seq_t *seq = NULL;      /* dynamic array of terms      */
static idx_t seq_len = 0;      /* number of terms currently   */
static idx_t seq_cap = 0;      /* current capacity of seq     */

/* binary search: true iff value exists in sorted array[0..len-1] */
static bool contains(const seq_t *array, idx_t len, seq_t value)
{
    idx_t low = 0;
    idx_t high = len;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        seq_t midval = array[mid];
        if (midval < value)
            low = mid + 1;
        else if (midval > value)
            high = mid;
        else
            return true;
    }
    return false;
}

/* test if candidate+1 can be expressed as a product of two previous terms */
static bool is_forbidden(seq_t candidate, idx_t len, const seq_t *array)
{
    seq_t target = candidate + (seq_t)1;          /* a(i)*a(j) */
    seq_t half = target / (seq_t)2;
    for (idx_t idx = 0; idx < len && array[idx] <= half; ++idx) {
        seq_t a = array[idx];
        if (target % a == (seq_t)0) {
            seq_t b = target / a;
            if (b != a && contains(array, len, b))
                return true; /* forbidden */
        }
    }
    return false;
}

/* ensure the static array has space for at least one more term */
static void grow_if_needed(void)
{
    if (seq_len >= seq_cap) {
        size_t new_cap = seq_cap ? (size_t)seq_cap * 2 : (size_t)8;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = (idx_t)new_cap;
    }
}

/* public entry function */
seq_t a(idx_t n)
{
    /* initialise with the two known values only once */
    if (seq == NULL) {
        seq_cap = 8;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq_len = 0;
        seq[seq_len++] = (seq_t)2;
        seq[seq_len++] = (seq_t)4;
    }

    /* extend memoised sequence up to the requested index */
    while (seq_len < n) {
        seq_t candidate = seq[seq_len - 1] + (seq_t)1;
        while (is_forbidden(candidate, seq_len, seq))
            candidate++;                     /* try next integer */

        grow_if_needed();                     /* make room */
        seq[seq_len++] = candidate;           /* store new term */
    }

    return seq[n - 1];
}
