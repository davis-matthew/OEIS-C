/*
 * A066567  Numbers that when incremented by the product of their digits
 *          produce a square.
 * https://oeis.org/A066567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   the n‑th integer m such that m + digit_product(m) is a perfect
 *            square. The algorithm scans integers in increasing order and
 *            memoises the found terms, allowing any order of queries
 * Range:     u8 n <= 3, u16 n <= 93, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_product, floor_sqrt, grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/digit_product.h"
#include "../utils/floor_sqrt.h"
#include "../utils/grow_seq_array.h"

static seq_t *seq_vals    = NULL;   /* array of found terms          */
static size_t  seq_len    = 0;      /* number of terms stored        */
static size_t  seq_cap    = 0;      /* current allocation size       */
static seq_t   next_candidate = 2;  /* smallest integer not yet tested */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for indices < 1 */

    /* Generate terms until we have at least n of them. */
    while (seq_len < (size_t)n) {
        seq_t m   = next_candidate;                 /* candidate integer   */
        seq_t prod = digit_product(m);             /* product of digits   */
        seq_t sum  = m + prod;                     /* m + product         */
        seq_t r    = floor_sqrt(sum);              /* ⌊√sum⌋             */
        seq_t r2   = r * r;                         /* square of root      */

        if (r2 == sum) {                           /* perfect square?    */
            /* Grow storage if necessary. */
            if (seq_len == seq_cap) {
                grow_seq_array(&seq_vals, &seq_cap, seq_len + 1);
            }
            seq_vals[seq_len++] = m;                /* store the term     */
        }

        next_candidate++;                          /* advance to next int */
    }

    return seq_vals[n - 1];                          /* 1‑based access     */
}
