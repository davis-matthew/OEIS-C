/*
 * A133779  Irregular array: n-th row lists the "isolated divisors" of n. A
 *          positive divisor k of n is isolated if neither k-1 nor k+1 divides
 *          n.
 * https://oeis.org/A133779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 1073, u16 n <= 9264, u32 n <= 9264, u64 n <= 9264,
 *            u128 n <= 9264, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: builds the rows of the irregular array one by one,
   appending the isolated divisors of each n.  For each n a(n) is the
   next value in that row (or 0 for n=2, which has no isolated divisors). */

#include <stdlib.h>
#include <stdbool.h>

/* Ensure the array has at least 'needed' capacity, reallocating if
   necessary.  'array' points to the current array pointer and
   'cap' to the current capacity. */
static void ensure_capacity(size_t needed, size_t *cap, seq_t **array)
{
    if (*cap < needed) {
        size_t newcap = (*cap == 0) ? 1 : (*cap << 1);
        while (newcap < needed) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(*array, newcap * sizeof(seq_t));
        if (!tmp) exit(1);
        *array = tmp;
        *cap   = newcap;
    }
}

/* Append the isolated divisors of a single row.  If the row has none,
   a placeholder 0 is appended instead. */
static void append_row(idx_t row, seq_t **seq, size_t *len, size_t *cap)
{
    bool found = false;

    /* Temporary storage for the larger divisors (n/d), collected in reverse. */
    seq_t *large = NULL;
    size_t large_cap = 0;
    size_t large_len = 0;

    for (idx_t d = 1; d <= row / d; ++d) {
        if (row % d != 0) continue;

        /* Small divisor d. */
        bool iso = true;
        if (d > 1 && row % (d - 1) == 0) iso = false;
        if (row % (d + 1) == 0) iso = false;
        if (iso) {
            ensure_capacity(*len + 1, cap, seq);
            (*seq)[(*len)++] = (seq_t)d;
            found = true;
        }

        /* Complementary divisor n/d (if distinct). */
        idx_t d2 = row / d;
        if (d2 == d) continue;
        bool iso2 = true;
        if (d2 > 1 && row % (d2 - 1) == 0) iso2 = false;
        if (row % (d2 + 1) == 0) iso2 = false;
        if (iso2) {
            if (large_len == large_cap) {
                size_t newcap = (large_cap == 0) ? 8 : (large_cap << 1);
                seq_t *tmp = (seq_t *)realloc(large, newcap * sizeof(seq_t));
                if (!tmp) exit(1);
                large = tmp;
                large_cap = newcap;
            }
            large[large_len++] = (seq_t)d2;
            found = true;
        }
    }

    /* Append the larger divisors in ascending order (reverse the
       collected list). */
    for (size_t j = large_len; j-- > 0; ) {
        ensure_capacity(*len + 1, cap, seq);
        (*seq)[(*len)++] = large[j];
    }
    free(large);

    /* If no isolated divisors were found, store the placeholder 0. */
    if (!found) {
        ensure_capacity(*len + 1, cap, seq);
        (*seq)[(*len)++] = (seq_t)0;
    }
}

/* Static state used for memoization. */
static seq_t *seq_arr = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;
static idx_t next_row = 1;

/* Extends the stored sequence so that it contains at least 'n' terms. */
static void extend_sequence(idx_t n)
{
    while (seq_len < (size_t)n) {
        append_row(next_row, &seq_arr, &seq_len, &seq_cap);
        ++next_row;
    }
}

/* External linkage function required by the build. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    extend_sequence(n);
    return seq_arr[n - 1];
}
