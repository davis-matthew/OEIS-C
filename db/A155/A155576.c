/*
 * A155576  Intersection of A000404 and A155716: N = a^2 + b^2 = c^2 + 6d^2
 *          for some positive integers a,b,c,d.
 * https://oeis.org/A155576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Intersect of sums: a(n) = the positive integers N that can be written
   both as a^2 + b^2 and as c^2 + 6 d^2 with a,b,c,d > 0.
   The function builds the sorted list of such N values on demand and
   returns the n‑th member (offset 1).  It grows the search bound
   exponentially until enough terms are found.  This algorithm is
   straightforward and works for any seq_t type that can be constructed
   from an unsigned integer.  */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

static seq_t *intersect = NULL;      /* sorted unique numbers found so far */
static idx_t  intersect_len = 0;     /* length of 'intersect' (thread‑unsafe) */
static idx_t  max_computed_L = 0;    /* upper bound up to which we have searched */

/* Rebuild the intersection list up to a given bound L (inclusive).  */
static void recompute_up_to(idx_t L)
{
    /* free previous result */
    if (intersect != NULL) {
        free(intersect);
        intersect = NULL;
        intersect_len = 0;
    }

    /* allocate boolean flags for the two representations */
    size_t sz = (size_t)L + 1;               /* L + 1 because we include L itself */
    uint8_t *sum_sq = (uint8_t*)calloc(sz, 1);   /* a^2 + b^2 */
    uint8_t *form6  = (uint8_t*)calloc(sz, 1);   /* c^2 + 6 d^2 */
    if (!sum_sq || !form6) {
        free(sum_sq);
        free(form6);
        return;                             /* allocation failed – caller will not find value */
    }

    /* Mark numbers representable as a^2 + b^2 with a,b >= 1 */
    for (idx_t a = 1; a * a <= L; ++a) {
        idx_t a2 = a * a;
        for (idx_t b = 1; a2 + b * b <= L; ++b) {
            sum_sq[a2 + b * b] = 1;
        }
    }

    /* Mark numbers representable as c^2 + 6 d^2 with c,d >= 1 */
    for (idx_t d = 1; 6 * d * d <= L; ++d) {
        idx_t d6 = 6 * d * d;
        for (idx_t c = 1; c * c + d6 <= L; ++c) {
            form6[c * c + d6] = 1;
        }
    }

    /* Build the intersection vector, expanding it as necessary */
    size_t cap = 128;
    intersect = (seq_t*)malloc(cap * sizeof(seq_t));
    if (intersect == NULL) {
        free(sum_sq);
        free(form6);
        return;
    }

    for (idx_t x = 1; x <= L; ++x) {
        if (sum_sq[x] && form6[x]) {
            if (intersect_len == cap) {
                cap <<= 1;
                intersect = (seq_t*)realloc(intersect, cap * sizeof(seq_t));
                if (intersect == NULL) {
                    free(sum_sq);
                    free(form6);
                    intersect_len = 0;
                    return;
                }
            }
            intersect[intersect_len++] = (seq_t)x;
        }
    }

    free(sum_sq);
    free(form6);
    max_computed_L = L;
}

/* Return the n-th term of the sequence (offset 1).  */
seq_t a(idx_t n)
{
    if (n < 1U) {
        return 0;    /* undefined for n < 1 */
    }

    /* Lazily generate terms until we have at least n of them. */
    if (intersect_len < n) {
        idx_t L = (max_computed_L == 0U) ? 256 : max_computed_L;
        while (intersect_len < n) {
            L <<= 1;                 /* double the upper bound */
            recompute_up_to(L);
        }
    }

    /* after the loop we are guaranteed intersect_len >= n */
    return intersect[n - 1];
}
