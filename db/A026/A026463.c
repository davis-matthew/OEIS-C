/*
 * A026463  a(n) = least positive integer > a(n-1) and not a(i)*a(j)-2 for
 *          1<=i<=j<=n, n >= 2.
 * https://oeis.org/A026463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A026463:
   a(1) = 2, a(2) = 4 and for n >= 3,
   a(n) = least integer > a(n-1) that is NOT of the form
           a(i)*a(j) - 2 for any 1 <= i <= j < n.
   The implementation searches forward and tests candidates
   by checking whether x+2 has a factor pair consisting of
   previously computed terms.  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* static storage for the sequence */
static seq_t *seq      = NULL;   /* array of already computed terms          */
static size_t seq_cap  = 0;      /* capacity of seq                         */
static size_t seq_len  = 0;      /* number of terms stored (equiv. n-1)     */

static unsigned char *present = NULL;  /* bitmap of values that have been seen   */
static size_t present_cap    = 0;      /* allocated size for present             */

/* enlarge the sequence array to at least new_cap */
static void ensure_seq_cap(size_t new_cap)
{
    if (seq_cap >= new_cap) return;
    size_t new_len = (seq_cap == 0 ? 4 : seq_cap * 2);
    while (new_len < new_cap) new_len *= 2;
    seq = (seq_t *)realloc(seq, new_len * sizeof(seq_t));
    seq_cap = new_len;
}

/* enlarge the present bitmap to at least new_cap */
static void ensure_present_cap(size_t new_cap)
{
    if (present_cap >= new_cap) return;
    size_t new_len = (present_cap == 0 ? 4 : present_cap * 2);
    while (new_len < new_cap) new_len *= 2;
    present = (unsigned char *)realloc(present, new_len);
    /* initialise the new region to zero */
    for (size_t i = present_cap; i < new_len; ++i) present[i] = 0;
    present_cap = new_len;
}

/* add a new term to the sequence, updating both arrays */
static void add_term(seq_t val)
{
    ensure_seq_cap(seq_len + 1);
    seq[seq_len++] = val;
    /* ensure the bitmap can index this value */
    size_t idx = (size_t)val;     /* safe because values stay small for n <= 10000 */
    ensure_present_cap(idx + 1);
    present[idx] = 1;
}

/* test whether candidate x is forbidden:
   i.e., whether x+2 can be written as a(i)*a(j) with i <= j < current length
*/
static bool is_forbidden(seq_t x)
{
    seq_t y = x + 2;
    seq_t sqrt_y = floor_sqrt(y);
    for (size_t k = 0; k < seq_len; ++k) {
        seq_t ai = seq[k];
        if (ai > sqrt_y) break;
        if (y % ai == 0) {
            seq_t other = y / ai;
            size_t idx = (size_t)other;
            if (idx < present_cap && present[idx]) {
                return true; /* forbidden */
            }
        }
    }
    return false;          /* allowed */
}

/* public function */
seq_t a(idx_t n)
{
    /* handle base cases */
    if (n == 1) return 2;
    if (n == 2) return 4;

    /* initialise sequence on first call */
    if (seq_len == 0) {
        ensure_seq_cap(8);
        ensure_present_cap(32);
        add_term(2);          /* a(1) */
        add_term(4);          /* a(2) */
    }

    /* compute forwards until we have enough terms */
    seq_t candidate = seq[seq_len - 1] + 1;
    while (seq_len < (size_t)n) {
        if (!is_forbidden(candidate)) {
            add_term(candidate);
        }
        candidate++;
    }
    return seq[n - 1];
}
