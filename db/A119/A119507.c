/*
 * A119507  Members of A016052 whose digit sum is three.
 * https://oeis.org/A119507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A119507:  Terms of A016052 that have digit sum 3.  
 * a(n) is the n‑th member of A016052 whose digit sum equals 3.  
 * The underlying recurrence is:  A016052:  A(n+1) = A(n) + digit_sum(A(n)), A(1)=3.  */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"

/* static cache for generated terms of the subsequence */
static bool      initialized = false;   /* have we seeded the sequence?          */
static seq_t     last_val;              /* last generated term of A016052        */
static idx_t     last_index;            /* 1‑based index of last_val             */
static seq_t    *vals_cache = NULL;      /* stored A016052 terms with ds = 3      */
static size_t   val_len   = 0;           /* number of stored values               */
static size_t   val_cap   = 0;           /* allocated capacity of vals_cache      */

/* ensure that vals_cache can hold at least 'needed' values */
static void
ensure_val_capacity(size_t needed)
{
    if (needed <= val_cap)
        return;
    size_t newcap = val_cap ? val_cap * 2 : 1;
    while (newcap < needed)
        newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(vals_cache, newcap * sizeof(seq_t));
    vals_cache = tmp;
    val_cap = newcap;
}

/* generate terms until we have at least 'n' subsequence values */
static void
generate_until(idx_t n)
{
    if (!initialized) {
        /* seed with the first term of A016052 */
        last_val   = (seq_t)3;
        last_index = 1;

        ensure_val_capacity(1);
        vals_cache[0] = last_val;    /* 3 has digit sum 3 */
        val_len = 1;

        initialized = true;
    }

    while (val_len < (size_t)n) {
        /* produce next term of A016052 */
        seq_t next = last_val + digit_sum(last_val);
        last_index++;

        /* store the term if its digit sum is 3 */
        if (digit_sum(next) == (seq_t)3) {
            ensure_val_capacity(val_len + 1);
            vals_cache[val_len++] = next;
        }

        last_val = next;   /* advance to next term */
    }
}

/* public function required by the build */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined; safe fallback */
    generate_until(n);
    return vals_cache[n - 1];
}
