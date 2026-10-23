/*
 * A022956  Unique increasing sequence satisfying a(n) = a(n-4) + c(n-4);
 *          where c( ) is complement of a( ).
 * https://oeis.org/A022956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 683, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Recurrence: a(n) = a(n‑4) + c(n‑4)  (n > 4),  a(1)=1, a(2)=2, a(3)=3, a(4)=4;
   c(k) is the k‑th integer NOT in the sequence a(1…), i.e. the complement of a. */

static seq_t *seq        = NULL;   /* known terms a(1) … a(len) (0‑based) */
static size_t seq_len    = 0;      /* number of terms stored in seq */
static size_t seq_cap    = 0;      /* current capacity of seq */

static seq_t *comp       = NULL;   /* known complement terms c(1) … c(len) (0‑based) */
static size_t comp_len   = 0;      /* number of terms stored in comp */
static size_t comp_cap   = 0;      /* current capacity of comp */

static seq_t comp_next   = 1;      /* next integer to test for inclusion in c */

/* Ensure seq has capacity for at least 'needed' elements */
static inline void grow_seq(size_t needed)
{
    if (seq_cap < needed)
    {
        size_t new_cap = seq_cap ? seq_cap * 2 : 4;
        if (new_cap < needed) new_cap = needed;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
}

/* Ensure comp has capacity for at least 'needed' elements */
static inline void grow_comp(size_t needed)
{
    if (comp_cap < needed)
    {
        size_t new_cap = comp_cap ? comp_cap * 2 : 4;
        if (new_cap < needed) new_cap = needed;
        comp = (seq_t *)realloc(comp, new_cap * sizeof(seq_t));
        comp_cap = new_cap;
    }
}

/* Binary search: true iff 'val' appears in the sorted array arr[0..len-1] */
static inline bool contains_seq(const seq_t *arr, size_t len, seq_t val)
{
    size_t lo = 0, hi = len;
    while (lo < hi)
    {
        size_t mid = (lo + hi) / 2;
        if (arr[mid] < val)
            lo = mid + 1;
        else if (arr[mid] > val)
            hi = mid;
        else
            return true;
    }
    return false;
}

/* Return the n-th term of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined beyond offset */

    /* Grow state until we have a(n) */
    while (seq_len < (size_t)n)
    {
        if (seq_len < 4)                    /* initial seed terms */
        {
            grow_seq(seq_len + 1);
            seq[seq_len] = (seq_t)(seq_len + 1);
            ++seq_len;
        }
        else                                /* recursive step */
        {
            /* Ensure complement array has terms up to index seq_len-4 */
            while (comp_len <= seq_len - 4)
            {
                seq_t val = comp_next;
                while (contains_seq(seq, seq_len, val))
                    ++val;                 /* skip numbers already in a */

                grow_comp(comp_len + 1);
                comp[comp_len] = val;
                ++comp_len;
                comp_next = val + 1;
            }

            /* a(k) = a(k-4) + c(k-4) */
            seq_t new_val = seq[seq_len - 4] + comp[seq_len - 4];
            grow_seq(seq_len + 1);
            seq[seq_len] = new_val;
            ++seq_len;
        }
    }

    return seq[n - 1];                      /* array is 0‑based */
}
