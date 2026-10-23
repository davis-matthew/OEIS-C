/*
 * A172177  Triangle T(n, k) = 1 + abs(n! - k!)*abs(n! - (n-k)!), read by
 *          rows.
 * https://oeis.org/A172177
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 1 + |n! - k!| * |n! - (n-k)!| (offset 0)
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 45, u64 n <= 91, u128 n <= 231,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static seq_t *fact_cache = NULL;          /* factorial cache: fact_cache[i] = i! */
static idx_t fact_cache_len = 0;          /* number of cached factorials */

/* ensure factorials up to n are cached */
static void ensure_factorial_cache(idx_t n)
{
    if (!fact_cache) {
        /* allocate space for at least 200 factorials (sufficient for n <= 10000) */
        fact_cache = (seq_t*)malloc(sizeof(seq_t) * 200);
        fact_cache[0] = (seq_t)1;
        fact_cache_len = 1;
    }
    while (fact_cache_len <= n) {
        /* expand cache if necessary */
        if (fact_cache_len >= 200) {
            fact_cache = (seq_t*)realloc(fact_cache,
                                         sizeof(seq_t) * (fact_cache_len + 200));
        }
        fact_cache[fact_cache_len] =
            fact_cache[fact_cache_len - 1] * (seq_t)fact_cache_len;
        fact_cache_len++;
    }
}

seq_t a(idx_t n)
{
    /* find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) r++;

    /* position k within the row */
    idx_t prev_terms = r * (r + 1) / 2;
    idx_t k = n - prev_terms;

    /* create factorial cache up to r */
    ensure_factorial_cache(r);

    seq_t fact_r   = fact_cache[r];
    seq_t fact_k   = fact_cache[k];
    seq_t fact_rm  = fact_cache[r - k];

    seq_t diff1 = (fact_r > fact_k) ? (fact_r - fact_k) : (fact_k - fact_r);
    seq_t diff2 = (fact_r > fact_rm) ? (fact_r - fact_rm) : (fact_rm - fact_r);

    return (seq_t)1 + diff1 * diff2;
}
