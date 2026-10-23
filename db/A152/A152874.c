/*
 * A152874  Triangle read by rows: T(n,k) is the number of permutations of
 *          {1,2,...,n} with k parity changes (n>=2; 1<=k <=n-1); the
 *          permutation 372185946 has 5 parity changes: 37-2-1-8-59-46.
 * https://oeis.org/A152874
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth element of the triangle read by rows of T(n,k),
 *            where T(n,k) counts permutations of {1,…,n} with k parity
 *            changes. For n≥2 this gives the sequence A152874
 * Range:     u8 n <= 13, u16 n <= 31, u32 n <= 82, u64 n <= 217,
 *            u128 n <= 574, big any n
 * Helpers:   _floor_isqrt_u64, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/binomial.h"
#include "../utils/_floor_isqrt_u64.h"


static seq_t *fact_cache = NULL;
static size_t fact_cache_len = 0;
static size_t fact_cache_cap = 0;

static void ensure_factorial_cache(idx_t limit)
{
    if ((size_t)limit < fact_cache_len)
        return;
    size_t needed = (size_t)limit + 1;
    if (needed > fact_cache_cap) {
        size_t new_cap = fact_cache_cap ? fact_cache_cap * 2 : 16;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(fact_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        fact_cache = tmp;
        fact_cache_cap = new_cap;
    }
    for (idx_t i = fact_cache_len; i <= limit; ++i) {
        if (i == 0)
            fact_cache[0] = (seq_t)1;
        else
            fact_cache[i] = fact_cache[i-1] * (seq_t)i;
    }
    fact_cache_len = (size_t)limit + 1;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;                /* undefined for n<2 */

    /* index in flattened triangle (zero‑based, offset 2) */
    idx_t x = n - 2;

    /* row r: smallest integer with r(r‑1)/2 > x */
    idx_t inside = 1 + (idx_t)8 * (x + 1);
    idx_t sq = _floor_isqrt_u64(inside);
    idx_t r = (1 + sq) / 2;
    if (r * (r - 1) / 2 <= x) r++;              /* adjust if triangle root was low */

    /* previous cumulative terms before row r */
    idx_t prev = (r - 1) * (r - 2) / 2;
    idx_t pos = x - prev;                       /* zero‑based position in the row */
    idx_t k = pos + 1;                          /* column index (1‑based) */

    /* compute T(r,k) ------------------------------------------- */
    if ((r & 1) == 0) {                         /* even row: r = 2*m */
        idx_t m = r >> 1;                       /* m = r/2 */
        ensure_factorial_cache(m);
        seq_t fm = fact_cache[m];
        seq_t tmp = fm * fm;                    /* (m!)^2 */

        seq_t a_val;
        if (k & 1) {                            /* k odd */
            idx_t t = (k - 1) >> 1;
            seq_t b = binomial(m - 1, t);
            a_val = (seq_t)2 * b * b;
        } else {                                /* k even */
            idx_t t = k >> 1;
            seq_t b1 = binomial(m - 1, t - 1);
            seq_t b2 = binomial(m - 1, t);
            a_val = (seq_t)2 * b1 * b2;
        }
        return tmp * a_val;
    } else {                                    /* odd row: r = 2*m + 1 */
        idx_t m = (r - 1) >> 1;                 /* m = (r‑1)/2 */
        ensure_factorial_cache(m + 1);           /* need (m+1)! */

        seq_t fm  = fact_cache[m];
        seq_t fmp = fact_cache[m + 1];
        seq_t prod = fm * fmp;                  /* m! * (m+1)! */

        seq_t b_val;
        if (k & 1) {                            /* k odd */
            idx_t t = (k - 1) >> 1;
            seq_t b1 = binomial(m, t);
            seq_t b2 = binomial(m - 1, t);
            b_val = (seq_t)2 * b1 * b2;
        } else {                                /* k even */
            idx_t t = k >> 1;
            seq_t term1 = binomial(m, t) * binomial(m - 1, t - 1);
            seq_t term2 = binomial(m, t - 1) * binomial(m - 1, t);
            b_val = term1 + term2;
        }
        return prod * b_val;
    }
}
