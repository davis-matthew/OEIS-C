/*
 * A155453  Triangle read by rows: T(n,k) = 1 + n! - k! - (n - k)! + k!*(n -
 *          k)!.
 * https://oeis.org/A155453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the n-th entry of the triangle T(n,k) = 1 + n! - k! - (n
 *            - k)! + k! * (n-k)! The triangle is read by rows, starting with
 *            row 0. a(n) gives the flattened sequence (row-major)
 * Range:     u8 n <= 20, u16 n <= 44, u32 n <= 90, u64 n <= 230,
 *            u128 n <= 629, big any n
 * Helpers:   index_to_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include "../utils/index_to_row.h"

/* cache of factorials */
static seq_t *fact_cache = NULL;
static idx_t fact_cache_cap = 0;

/* enlarge factorial cache to include n! */
static void ensure_fact(idx_t n)
{
    if (n < fact_cache_cap)
        return;

    idx_t new_cap = n + 1;
    seq_t *new_cache = (seq_t *)realloc(fact_cache, new_cap * sizeof(seq_t));
    if (new_cache == NULL)
        abort();                      /* out of memory */

    fact_cache = new_cache;

    /* initialise when cache is empty */
    if (fact_cache_cap == 0) {
        fact_cache[0] = 1;
        fact_cache_cap = 1;
    }

    /* compute new factorials */
    for (idx_t i = fact_cache_cap; i < new_cap; ++i)
        fact_cache[i] = fact_cache[i - 1] * (seq_t)i;

    fact_cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* determine (row, column) */
    idx_t row = index_to_row(n);
    idx_t offset = row * (row + 1) / 2;
    idx_t col   = n - offset;           /* 0 <= col <= row */

    /* ensure factorials up to needed values */
    ensure_fact(row);
    ensure_fact(col);
    ensure_fact(row - col);

    seq_t fact_n   = fact_cache[row];
    seq_t fact_k   = fact_cache[col];
    seq_t fact_nk  = fact_cache[row - col];

    /* compute 1 + n! - k! - (n-k)! + k!*(n-k)! */
    seq_t prod = fact_k * fact_nk;
    seq_t result = prod + fact_n + 1;
    result -= fact_k;
    result -= fact_nk;

    return result;
}
