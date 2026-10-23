/*
 * A039397  Numbers whose base-10 representation has the same nonzero number
 *          of 0's and 5's.
 * https://oeis.org/A039397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039397: Numbers whose base‑10 representation has the
 * same non‑zero number of 0's and 5's.
 * a(n) = n-th positive integer such that count_zero(digits)=count_five(digits)>0
 */

#include <stdlib.h>
#include <stdint.h>

/* The tests provide get_digits in utils, but we implement digit counting
   directly to avoid any external dependency. */
 
static seq_t *cache = NULL;          /* cached sequence values */
static idx_t  size  = 0;             /* number of values cached */
static idx_t  cap   = 0;             /* current capacity of cache */
static idx_t  last_checked = 0;      /* last integer examined */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* behaviour below the offset is irrelevant */

    while (size < n) {
        idx_t cand = last_checked + 1;   /* next integer to test   */
        idx_t temp = cand;

        unsigned zeros = 0, fives = 0;
        /* Count 0's and 5's in base‑10 representation. */
        while (temp > 0) {
            unsigned digit = temp % 10;
            if (digit == 0) ++zeros;
            else if (digit == 5) ++fives;
            temp /= 10;
        }

        /* Require equal non‑zero counts of 0 and 5. */
        if (zeros > 0 && zeros == fives) {
            if (size == cap) {
                cap = cap ? cap * 2 : 32;     /* grow cache if needed */
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                if (!cache) abort();          /* allocation failed */
            }
            cache[size++] = (seq_t)cand;       /* store the valid term   */
        }
        last_checked = cand;
    }

    return cache[n - 1];   /* 1‑based indexing */
}
