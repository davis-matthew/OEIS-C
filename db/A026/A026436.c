/*
 * A026436  a(n) = least positive integer > a(n-1) and not a(i)*a(j)-1 for
 *          1<=i<j<=n, with initial terms 3,4.
 * https://oeis.org/A026436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search algorithm for OEIS A026436:
   a(1)=3, a(2)=4.  For n≥3, a(n) is the least positive integer > a(n‑1)
   that cannot be expressed as a(i)*a(j)-1 for any 1≤i<j< n.  The
   function builds the sequence on demand, memoizing all previous terms. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

static seq_t *seq = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

/* Grow the storage array to hold at least 'needed' elements. */
static void ensure_capacity(size_t needed)
{
    if (seq_cap >= needed) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq = tmp;
    seq_cap = new_cap;
}

/* Binary search for 'val' in the sorted array 'array' of length 'len'.
   Returns the index if found, or (idx_t)-1 if not found. */
static idx_t find_index(const seq_t *array, size_t len, seq_t val)
{
    if (len == 0) return (idx_t)-1;
    idx_t left = 0, right = (idx_t)(len - 1);
    while (left <= right)
    {
        idx_t mid = left + ((right - left) >> 1);
        seq_t midv = array[mid];
        if (midv == val) return mid;
        if (midv < val)  left = mid + 1;
        else             right = mid - 1;
    }
    return (idx_t)-1;   /* not found */
}

/* Compute the n-th term of A026436 (1‑based index). */
seq_t a(idx_t n)
{
    /* Build terms up to n on demand. */
    while (seq_len < n)
    {
        if (seq_len == 0)   /* a(1) = 3 */
        {
            ensure_capacity(1);
            seq[0] = (seq_t)3;
            seq_len = 1;
            continue;
        }
        if (seq_len == 1)   /* a(2) = 4 */
        {
            ensure_capacity(2);
            seq[1] = (seq_t)4;
            seq_len = 2;
            continue;
        }

        /* Find the next term. */
        seq_t candidate = seq[seq_len - 1] + (seq_t)1;
        for (;;)
        {
            seq_t prod_plus_one = candidate + (seq_t)1;
            seq_t limit = floor_sqrt(prod_plus_one);
            bool forbidden = false;

            /* Scan divisors d of prod_plus_one up to sqrt(prod_plus_one). */
            for (idx_t i = 0; i < seq_len; ++i)
            {
                seq_t d = seq[i];
                if (d > limit) break;
                if (prod_plus_one % d == 0)
                {
                    seq_t q = prod_plus_one / d;
                    if (q == d) continue;                /* distinct indices required */
                    if (find_index(seq, seq_len, q) != (idx_t)-1)
                    {
                        forbidden = true;
                        break;
                    }
                }
            }

            if (!forbidden) break;   /* candidate is valid */
            ++candidate;
        }

        /* Append the valid candidate. */
        ensure_capacity(seq_len + 1);
        seq[seq_len++] = candidate;
    }

    return seq[n - 1];
}
