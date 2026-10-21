/*
 * A080573  Number of ways writing n as a sum of two distinct Ulam numbers.
 * https://oeis.org/A080573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
Sequence A080573: Number of ways writing n as a sum of two distinct Ulam numbers.
The value is computed by generating all Ulam numbers up to n and counting the
distinct unordered pairs that sum to n. */
#include <stdlib.h>

static idx_t   current_limit = 0;   /* largest number examined when building Ulam list */
static size_t  ulam_len      = 0;   /* number of Ulam numbers currently stored      */
static size_t  ulam_cap      = 0;   /* capacity of the ulam array                   */
static idx_t  *ulam          = NULL;/* sorted list of Ulam numbers                  */

/* Ensure that Ulam numbers are known up to at least `limit`. */
static void ensure_ulam_up_to(idx_t limit)
{
    /* Seed the list with the first two Ulam numbers if not already done. */
    if (ulam_len == 0) {
        ulam_cap = 4;
        ulam = (idx_t *)realloc(ulam, ulam_cap * sizeof(idx_t));
        if (!ulam) exit(1);
        ulam[0] = 1;
        ulam[1] = 2;
        ulam_len = 2;
        current_limit = 2;
    }

    while (current_limit < limit) {
        idx_t candidate = current_limit + 1;
        seq_t ways = 0;

        /* Count the number of distinct pairs of existing Ulam numbers
           that sum to `candidate`. Stop early if more than one pair
           is found because then it is not a Ulam number. */
        for (size_t i = 0; i < ulam_len; ++i) {
            for (size_t j = i + 1; j < ulam_len; ++j) {
                idx_t sum = ulam[i] + ulam[j];
                if (sum == candidate) {
                    ++ways;
                    if (ways > 1) goto candidate_done;
                } else if (sum > candidate) {
                    break;
                }
            }
        }
candidate_done:
        if (ways == 1) {                 /* candidate is a Ulam number */
            if (ulam_len == ulam_cap) {   /* grow the array if needed   */
                ulam_cap = ulam_cap ? ulam_cap * 2 : 4;
                ulam = (idx_t *)realloc(ulam, ulam_cap * sizeof(idx_t));
                if (!ulam) exit(1);
            }
            ulam[ulam_len++] = candidate;
        }
        current_limit = candidate;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* defined only for n ≥ 1 */

    ensure_ulam_up_to(n);

    if (ulam_len < 2) return 0;          /* need at least two Ulam numbers */

    seq_t count = 0;
    size_t i = 0;
    size_t j = ulam_len - 1;

    /* Count unordered pairs of distinct Ulam numbers summing to n using
       a two‑pointer technique on the sorted list. */
    while (i < j) {
        idx_t sum = ulam[i] + ulam[j];
        if (sum == n) {
            ++count;
            ++i;
            --j;
        } else if (sum < n) {
            ++i;
        } else {          /* sum > n */
            --j;
        }
    }
    return count;
}
