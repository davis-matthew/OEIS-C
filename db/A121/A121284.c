/*
 * A121284  Triangle T(n,k), 1<=k<=n, read by rows given by T(n,k) =
 *          A003266(n)/A000045(k).
 * https://oeis.org/A121284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (∏_{i=1..r} Fib(i)) / Fib(k) where n is the linear index
 *            in the triangle T(n,k)=A003266(n)/A000045(k), r is the row
 *            number (1‑based) and k the column (1‑based) such that n =
 *            1+2+…+r‑1 + k. Fib(i) is the i‑th Fibonacci number (Fib(1)=1,
 *            Fib(2)=1)
 * Range:     u8 n <= 21, u16 n <= 36, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* static caches for Fibonacci numbers and their running products */
static seq_t *fib_cache  = NULL;   /* fib_cache[i] = Fib(i) */
static seq_t *prod_cache = NULL;   /* prod_cache[i] = ∏_{j=1..i} Fib(j) */
static size_t cache_cap = 0;       /* allocated capacity of the caches */
static size_t cache_size = 0;      /* largest index currently cached */

/* enlarge the caches to hold at least up_to (inclusive) */
static void ensure_cache(size_t up_to)
{
    if (up_to <= cache_size) return;

    size_t new_cap = cache_cap;
    if (new_cap == 0) new_cap = 8;
    while (new_cap <= up_to) new_cap <<= 1;

    fib_cache  = (seq_t *)realloc(fib_cache,  new_cap * sizeof(seq_t));
    prod_cache = (seq_t *)realloc(prod_cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;

    for (size_t i = cache_size + 1; i <= up_to; ++i) {
        if (i == 1) {
            fib_cache[1] = (seq_t)1;
            prod_cache[1] = fib_cache[1];
        } else if (i == 2) {
            fib_cache[2] = (seq_t)1;
            prod_cache[2] = prod_cache[1] * fib_cache[2];
        } else {
            fib_cache[i] = fib_cache[i-1] + fib_cache[i-2];
            prod_cache[i] = prod_cache[i-1] * fib_cache[i];
        }
    }

    cache_size = up_to;
}

/* binary search for the row r such that r*(r+1)/2 >= n */
static idx_t row_from_index(idx_t n)
{
    idx_t low = 1, high = 1;

    while (((seq_t)high * ((seq_t)high + 1)) / 2 < n) {
        if (high > (idx_t)(-1) / 2) break;   /* guard against overflow */
        high <<= 1;
    }

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        seq_t tri = ((seq_t)mid * ((seq_t)mid + 1)) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }

    return low;
}

/* main sequence function required by the tester */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1 */

    idx_t row = row_from_index(n);

    seq_t prev_terms = ((seq_t)(row - 1) * (seq_t)row) / 2;
    idx_t col = (idx_t)(n - prev_terms);     /* 1 <= col <= row */

    ensure_cache((size_t)row);

    return prod_cache[row] / fib_cache[col];
}
