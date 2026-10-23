/*
 * A164710  A positive integer n is included if all runs of 0's in binary n
 *          are of the same length.
 * https://oeis.org/A164710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 154, u16 n <= 5859, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A164710: all runs of 0's in the binary expansion of the
   term have the same length.  The function performs a brute‑force
   search and memoises the results. */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Test whether x satisfies the condition. */
static bool good(seq_t x)
{
    int first_len = -1;      /* length of the first zero run */
    int current_len = 0;
    bool in_zero = false;    /* inside a run of zeros */

    while (x != 0) {
        if ((x & (seq_t)1) == 0) {            /* zero bit */
            if (!in_zero) {
                in_zero = true;
                current_len = 1;
            } else {
                current_len++;
            }
        } else {                              /* one bit */
            if (in_zero) {
                if (first_len == -1) {
                    first_len = current_len;
                } else if (current_len != first_len) {
                    return false;
                }
                in_zero = false;
            }
        }
        x >>= 1;
    }

    /* check last run if the number ended with zeros */
    if (in_zero) {
        if (first_len == -1) {
            first_len = current_len;
        } else if (current_len != first_len) {
            return false;
        }
    }

    return true;
}

/* Memoised cache */
static seq_t *seq_cache = NULL;
static size_t cache_cap = 0;
static idx_t seq_count = 0;          /* number of terms stored (>= 1)   */
static seq_t next_candidate = 1;     /* next number to test            */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;                 /* undefined for the sequence */
    }

    /* Grow cache if necessary */
    while (seq_count < n) {
        /* find next term satisfying the condition */
        while (!good(next_candidate)) {
            next_candidate += 1;
        }

        /* store the term */
        if (seq_count == cache_cap) {
            size_t new_cap = (cache_cap == 0 ? 4 : cache_cap * 2);
            seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();           /* allocation failed */
            seq_cache = tmp;
            cache_cap = new_cap;
        }
        seq_cache[seq_count] = next_candidate;
        seq_count += 1;
        next_candidate += 1;              /* start searching from the next integer */
    }

    return seq_cache[n - 1];
}
