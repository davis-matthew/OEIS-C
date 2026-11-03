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
 * Validated: standard, pass, 63 terms through n = 937 (2026-11-03)
 * Helpers:   digit_product, ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/digit_product.h"
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Helper to grow a sequence array of type seq_t. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len) return;
    size_t new_cap = (*size > 0) ? (*size << 1) : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *size = new_cap;
}

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
