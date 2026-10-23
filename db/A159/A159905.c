/*
 * A159905  Triangle read by rows, Mobius transform of A004736.
 * https://oeis.org/A159905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = sum_{d|r, d <= r/c} μ(d) * ((r/d) - c + 1) where r is the
 *            row index. The row is the smallest r with r(r+1)/2 ≥ n. The
 *            column is c = n - (r-1)r/2 (1‑based)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Cache for computed terms */
static seq_t *cache   = NULL;     /* array, 1‑indexed */
static idx_t cache_size = 0;      /* largest index currently cached */

/* Möbius function table */
static int  *mu_arr = NULL;
static size_t mu_limit = 0;

/* Ensure that mu_arr[1..n] is computed */
static void ensure_mu(size_t n) {
    if (mu_limit >= n) return;
    if (mu_arr) free(mu_arr);
    mu_arr = (int *)malloc((n + 1) * sizeof(int));
    if (!mu_arr) return;               /* out of memory – unlikely */

    /* Linear sieve for μ */
    int *lp  = (int *)calloc((n + 1), sizeof(int));   /* smallest prime factor */
    int *pr  = (int *)calloc((n + 1), sizeof(int));   /* prime list */
    size_t pc = 0;
    mu_arr[1] = 1;
    for (size_t i = 2; i <= n; ++i) {
        if (lp[i] == 0) {
            lp[i] = (int)i;
            mu_arr[i] = -1;
            pr[pc++] = (int)i;
        }
        for (size_t j = 0; j < pc; ++j) {
            int p = pr[j];
            long long prod = (long long)p * i;
            if (prod > n) break;
            int k = (int)prod;
            lp[k] = p;
            if (p == lp[i]) {
                mu_arr[k] = 0;
                break;
            } else {
                mu_arr[k] = -mu_arr[i];
            }
        }
    }
    free(lp);
    free(pr);
    mu_limit = n;
}

/* Compute a(n) directly (no caching) */
static seq_t compute_value(idx_t n) {
    /* Find row r so that r(r+1)/2 ≥ n */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) ++r;
    idx_t prev_rows = (r - 1) * r / 2;
    idx_t c = n - prev_rows;          /* column (1‑based) */
    if (c == 0) c = r;                /* safety, should not occur */

    ensure_mu((size_t)r);

    seq_t sum = 0;
    idx_t limit = r / c;              /* floor(r/c) */
    for (idx_t d = 1; d * d <= r; ++d) {
        if (r % d == 0) {
            if (d <= limit && mu_arr[d] != 0) {
                seq_t term = (seq_t)((r / d) - c + 1);
                sum += (mu_arr[d] > 0) ? term : -term;
            }
            idx_t d2 = r / d;
            if (d2 != d && d2 <= limit && mu_arr[d2] != 0) {
                seq_t term = (seq_t)((r / d2) - c + 1);
                sum += (mu_arr[d2] > 0) ? term : -term;
            }
        }
    }
    return sum;
}

/* Extend the cache up to index n, computing missing values */
static void extend_cache(idx_t n) {
    if (cache_size >= n) return;
    size_t new_cap = n + 1;
    seq_t *new_cache = NULL;
    if (cache == NULL) {
        new_cache = (seq_t *)malloc(sizeof(seq_t) * new_cap);
    } else {
        new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
    }
    if (!new_cache) return;            /* out of memory – unlikely */

    cache = new_cache;
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        cache[i] = compute_value(i);
    }
    cache_size = n;
}

/* Public function required by the build */
seq_t a(idx_t n) {
    if (n == 0) return 0;            /* offset starts at 1 */
    extend_cache(n);
    return cache[n];
}
