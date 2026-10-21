/*
 * A050241  Handsome numbers (A007532) representable in exactly two distinct
 *          ways (counting different powers of duplicated digits as distinct).
 * https://oeis.org/A050241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   dfs_rep, is_handsome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Enumeration: generates candidate numbers and counts representations
    that give exactly two distinct ways of writing the number as a sum
    of digit^exponent for each decimal digit in the number.
*/

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/dfs_rep.h"
#include "../utils/is_handsome.h"

/* Grow an array of seq_t elements. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_cap)
{
    seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    *arr   = tmp;
    *size  = new_cap;
}

/* Storage for the sequence (1‑based). */
static seq_t *handsome_arr = NULL;
static size_t handsome_len = 0;
static size_t handsome_cap = 0;

/* Ensure the sequence contains at least `upto` terms (1‑based). */
static void compute_up_to(idx_t upto)
{
    if (!handsome_arr) {
        handsome_cap = 256;
        handsome_arr = (seq_t *)malloc(handsome_cap * sizeof(seq_t));
        if (!handsome_arr) abort();
    }

    seq_t cand = 1;
    if (handsome_len > 0) cand = handsome_arr[handsome_len-1] + 1;

    while ((idx_t)handsome_len < upto) {
        if (is_handsome(cand)) {
            if (handsome_len == handsome_cap) {
                handsome_cap <<= 1;
                grow_seq_array(&handsome_arr, &handsome_cap, handsome_cap);
            }
            handsome_arr[handsome_len++] = cand;
        }
        ++cand;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* offset is 1, behaviour for n<1 undefined */
    compute_up_to(n);
    return handsome_arr[n-1];
}
