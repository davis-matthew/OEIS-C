/*
 * A136680  Triangle T(n, k) = f(k) for k < n+1, otherwise 0, where f(k) =
 *          f(k-1) + k^(k-2)*f(k-2) with f(0) = 0 and f(1) = 1, read by rows.
 * https://oeis.org/A136680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = f(k), where k = n - r*(r-1)/2, r = the unique integer
 *            with r*(r-1)/2 < n <= r*(r+1)/2. f(0)=0, f(1)=1,
 *            f(k)=f(k-1)+k^(k-2)*f(k-2)
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 35, u64 n <= 65, u128 n <= 104,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/big_pow.h"

static seq_t *f_cache = NULL;   /* cache for f(k) */
static size_t f_size = 0;       /* number of initialized entries */
static size_t f_cap  = 0;       /* allocated capacity */

static void ensure_f(size_t k)
{
    if (k < f_size) return;
    size_t need = k + 1;
    if (f_cap < need) {
        size_t new_cap = f_cap ? f_cap * 2 : 2;
        if (new_cap < need) new_cap = need;
        seq_t *tmp = (seq_t *)realloc(f_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory */
        f_cache = tmp;
        f_cap   = new_cap;
    }
    for (size_t i = f_size; i <= k; ++i) {
        if (i == 0) {
            f_cache[0] = 0;
        } else if (i == 1) {
            f_cache[1] = 1;
        } else {
            seq_t p = big_pow((seq_t)i, (seq_t)(i - 2));
            f_cache[i] = f_cache[i - 1] + p * f_cache[i - 2];
        }
    }
    f_size = need;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* below offset – undefined */
    /* Determine row r: r*(r-1)/2 < n <= r*(r+1)/2 */
    idx_t row = (idx_t)((sqrt(8.0 * (double)n + 1.0) - 1.0) / 2.0);
    if ((row * (row + 1)) / 2 < n) ++row;
    /* Column k (1‑based) */
    idx_t base = (row - 1) * row / 2;
    idx_t k    = n - base;
    ensure_f((size_t)k);
    return f_cache[(size_t)k];
}
