/*
 * A153391  G.f.: A(x) = F(x*G(x)^2) where F(x) = G(x*F(x)) = 1 + x*F(x)^3 is
 *          the g.f. of A001764 and G(x) = F(x/G(x)) = 1 + x*G(x)^2 is the
 *          g.f. of A000108 (Catalan).
 * https://oeis.org/A153391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for n = 0; a(n) = (1/n) * Σ_{k=1..n} (C(3k+1,k)/(3k+1))
 *            * k * C(2n,n-k) for n > 0 where C(a,b) = binomial coefficient
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *a_cache = NULL;   /* a(0)…a(max) */
static idx_t   a_size  = 0;
static seq_t *d_cache = NULL;   /* d(k) = (C(3k+1,k)/(3k+1)) * k   */
static idx_t   d_size  = 0;
static seq_t *binom_row = NULL;/* C(2n,0)…C(2n,n) for current n */
static idx_t   binom_size = 0;

static void ensure_a(idx_t n)
{
    if (n + 1 <= a_size) return;
    idx_t new_size = n + 1;
    /* Allocate or grow a_cache */
    if (!a_cache) a_cache = (seq_t*)malloc(new_size * sizeof(seq_t));
    else a_cache = (seq_t*)realloc(a_cache, new_size * sizeof(seq_t));
    for (idx_t i = a_size; i <= n; ++i) a_cache[i] = 0;
    if (a_size == 0) a_cache[0] = 1;   /* a(0) = 1 */
    a_size = new_size;
}

static void ensure_d(idx_t n)
{
    if (n >= d_size) {
        idx_t old = d_size;
        d_size = n + 1;
        if (!d_cache) d_cache = (seq_t*)malloc(d_size * sizeof(seq_t));
        else d_cache = (seq_t*)realloc(d_cache, d_size * sizeof(seq_t));
        for (idx_t k = old; k <= n; ++k) {
            seq_t comb = 1;
            idx_t M = 3 * k + 1;
            /* compute C(M, k) */
            for (idx_t r = 0; r < k; ++r)
                comb = comb * (seq_t)(M - r) / (seq_t)(r + 1);
            /* comb now holds C(M, k) */
            seq_t term = comb / (seq_t)M;   /* integer division, exact */
            term = term * (seq_t)k;
            d_cache[k] = term;
        }
    }
}

static void ensure_binom(idx_t n)
{
    if (n + 1 <= binom_size) return;
    idx_t new_size = n + 1;
    if (!binom_row) binom_row = (seq_t*)malloc(new_size * sizeof(seq_t));
    else binom_row = (seq_t*)realloc(binom_row, new_size * sizeof(seq_t));
    /* compute C(2n, r) for r = 0..n */
    seq_t comb = 1;
    for (idx_t r = 0; r <= n; ++r) {
        binom_row[r] = comb;
        if (r < n)
            comb = comb * (seq_t)(2 * n - r) / (seq_t)(r + 1);
    }
    binom_size = new_size;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    ensure_a(n);
    if (a_cache[n] != 0) return a_cache[n];
    ensure_d(n);
    ensure_binom(n);
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        sum += d_cache[k] * binom_row[n - k];
    }
    a_cache[n] = sum / (seq_t)n;   /* division exact as per formula */
    return a_cache[n];
}
