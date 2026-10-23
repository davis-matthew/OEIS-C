/*
 * A121229  Beginning with a(1) = 1 and a(2) = 2, a(n) is not equal to the
 *          product of two consecutive (distinct) earlier terms.
 * https://oeis.org/A121229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 242, u16 n <= 1147, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A121229:
   a(1)=1, a(2)=2.  For n>2, a(n) is the smallest integer greater than a(n‑1)
   that is not equal to a product of two consecutive earlier terms.
*/

#include <stdlib.h>
#include <stdbool.h>

/* ----------------------------------------------------------------------------- */
/* Internal storage for memoisation                                           */
/* ----------------------------------------------------------------------------- */
static seq_t *seq_buf   = NULL;   /* a(1)..a(k)                                    */
static size_t seq_len   = 0;      /* number of stored terms                         */
static size_t seq_cap   = 0;      /* capacity of seq_buf                            */

static seq_t *forb_buf  = NULL;   /* forbidden products a(i)*a(i+1) (sorted)      */
static size_t forb_len  = 0;      /* number of forbidden products                  */
static size_t forb_cap  = 0;      /* capacity of forb_buf                           */

/* ----------------------------------------------------------------------------- */
/* Ensure capacity of the sequence array                                        */
/* ----------------------------------------------------------------------------- */
static void ensure_seq_cap(size_t min_cap)
{
    if (seq_cap < min_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 4;
        if (new_cap < min_cap) new_cap = min_cap;
        seq_buf = (seq_t *)realloc(seq_buf, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
}

/* ----------------------------------------------------------------------------- */
/* Ensure capacity of the forbidden products array                               */
/* ----------------------------------------------------------------------------- */
static void ensure_forb_cap(size_t min_cap)
{
    if (forb_cap < min_cap) {
        size_t new_cap = forb_cap ? forb_cap * 2 : 8;
        if (new_cap < min_cap) new_cap = min_cap;
        forb_buf = (seq_t *)realloc(forb_buf, new_cap * sizeof(seq_t));
        forb_cap = new_cap;
    }
}

/* ----------------------------------------------------------------------------- */
/* Check whether x is in the sorted forbidden list (binary search).             */
/* ----------------------------------------------------------------------------- */
static bool forb_contains(seq_t x)
{
    size_t lo = 0, hi = forb_len;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (forb_buf[mid] < x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (lo < forb_len && forb_buf[lo] == x);
}

/* ----------------------------------------------------------------------------- */
/* Add a new product to the forbidden list (appended, keeping sorted order).    */
/* ----------------------------------------------------------------------------- */
static void forb_add(seq_t prod)
{
    ensure_forb_cap(forb_len + 1);
    forb_buf[forb_len++] = prod;   /* products are generated in increasing order */
}

/* ----------------------------------------------------------------------------- */
/* Compute a(n)                                                                  */
/* ----------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* guard, offset is 1 */

    /* Initialise the sequence with the first two terms if this is the first call */
    if (seq_len == 0) {
        ensure_seq_cap(8);
        seq_buf[0] = (seq_t)1;
        seq_buf[1] = (seq_t)2;
        seq_len = 2;

        /* The product of the first two terms is forbidden for later positions */
        ensure_forb_cap(2);
        forb_buf[0] = (seq_t)1 * (seq_t)2;
        forb_len  = 1;
    }

    /* Generate terms until we have at least n of them */
    while (seq_len < n) {
        /* Candidate: next integer after the current last term */
        seq_t cand = seq_buf[seq_len - 1] + (seq_t)1;

        /* Skip any candidates that are forbidden products */
        while (forb_contains(cand)) {
            cand += (seq_t)1;
        }

        /* Store the new term */
        ensure_seq_cap(seq_len + 1);
        seq_buf[seq_len] = cand;

        /* Add the product of the last two terms to the forbidden list */
        seq_t prev   = seq_buf[seq_len - 1];
        seq_t prod   = prev * cand;
        forb_add(prod);

        /* Advance to the next position */
        seq_len += 1;
    }

    return seq_buf[n - 1];
}
