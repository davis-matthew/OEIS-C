/*
 * A130465  Triangle where the n-th row consists of the n smallest positive
 *          integers which are coprime to the number of divisors of n.
 * https://oeis.org/A130465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   divisor_count_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * a(n) is the n-th entry of the triangle where the r-th row consists of the r
 * smallest positive integers coprime to τ(r) (the number of divisors of r).
 * We unfold the triangle in row‑major order:
 * a(1)=τ(1) row 1: [1]
 * a(2)=τ(1) row 2: [1,3]   …
 * The function computes a(n) for any n ≥ 1.
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/gcd.h"

static seq_t **row_cache = NULL;   /* row_cache[r] points to array of r coprimes */
static size_t cache_capacity = 0;  /* allocated size of row_cache (0‑indexed) */

static void ensure_cache_capacity(idx_t r)
{
    if (r+1 <= cache_capacity)
        return;
    /* grow to at least r+1, doubling each time improves amortised cost */
    size_t new_cap = cache_capacity ? cache_capacity : 4;
    while (new_cap < r+1) new_cap <<= 1;
    seq_t **new_ptr = (seq_t **)realloc(row_cache, new_cap * sizeof(seq_t *));
    if (!new_ptr) return; /* allocation failed; let program abort later */
    for (size_t i = cache_capacity; i < new_cap; ++i)
        new_ptr[i] = NULL;
    row_cache = new_ptr;
    cache_capacity = new_cap;
}

static void build_row(idx_t r)
{
    /* Ensure the cache is large enough */
    ensure_cache_capacity(r);
    if (row_cache[r]) return;   /* already built */

    /* Compute τ(r) */
    uint64_t tau = divisor_count_u64(r);
    seq_t tau_seq = (seq_t)tau;

    /* Allocate space for r elements */
    seq_t *row = (seq_t *)malloc(r * sizeof(seq_t));
    if (!row) return; /* allocation failed; the caller will get garbage - program aborts */

    idx_t cnt = 0;
    seq_t cand = 1;
    while (cnt < (idx_t)r)
    {
        if (gcd(cand, tau_seq) == (seq_t)1)
            row[cnt++] = cand;
        cand++;
    }
    row_cache[r] = row;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined but guard against zero */

    /* Find smallest r with r(r+1)/2 >= n */
    idx_t low = 1, high = n;   /* high is safe upper bound */
    while (low < high)
    {
        idx_t mid = low + ((high - low) >> 1);
        __uint128_t tri = (__uint128_t)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Position within the row (1‑based) */
    idx_t prev_tri = (idx_t)(((__uint128_t)(r-1) * r) / 2);
    idx_t pos = n - prev_tri;   /* 1‑based */

    /* Build row if necessary */
    build_row(r);

    /* Return the requested element */
    return row_cache[r][pos - 1];
}
