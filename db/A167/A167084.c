/*
 * A167084  Number of reduced words of length n in Coxeter group on 31
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^13 = I.
 * https://oeis.org/A167084
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=1}^{12} 29·a(n‑k) − 435·a(n‑13) + C(n) where C(n) =
 *            1 for n = 0, 2 for 1 ≤ n ≤ 12, 1 for n = 13, and C(n) = 0 for n
 *            ≥ 14. The first 13 terms are given directly, afterwards the
 *            homogeneous recurrence holds
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* cached values of a(n) */
static seq_t *cache = NULL;
static idx_t cache_size = 0;    /* first unused index in cache */

/* extend the cache up to index n (inclusive) */
static void extend_cache(idx_t n)
{
    if (cache_size > n)
        return;

    seq_t *new_cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
    if (!new_cache) {
        /* allocation failure – abort */
        exit(EXIT_FAILURE);
    }
    cache = new_cache;

    /* initial values, indices 0 … 12 */
    static const seq_t init[13] = {
        (seq_t)1ULL,        /* a(0) */
        (seq_t)31ULL,       /* a(1) */
        (seq_t)930ULL,      /* a(2) */
        (seq_t)27900ULL,    /* a(3) */
        (seq_t)837000ULL,   /* a(4) */
        (seq_t)25110000ULL, /* a(5) */
        (seq_t)753300000ULL,/* a(6) */
        (seq_t)22599000000ULL, /* a(7) */
        (seq_t)677970000000ULL, /* a(8) */
        (seq_t)20339100000000ULL, /* a(9) */
        (seq_t)610173000000000ULL, /* a(10) */
        (seq_t)18305190000000000ULL, /* a(11) */
        (seq_t)549155700000000000ULL  /* a(12) */
    };

    for (idx_t i = cache_size; i <= n; ++i) {
        if (i < 13) {
            cache[i] = init[(size_t)i];
        } else if (i == 13) {
            /* a(13) = 29·(a12+…+a1) − 435·a0 + 1 */
            seq_t sum = 0;
            for (idx_t k = 1; k <= 12; ++k) {
                sum += (seq_t)29 * cache[i - k];
            }
            sum -= (seq_t)435 * cache[i - 13];
            sum += (seq_t)1;           /* constant term for n = 13 */
            cache[i] = sum;
        } else {
            /* homogeneous recurrence for n ≥ 14 */
            seq_t sum = 0;
            for (idx_t k = 1; k <= 12; ++k) {
                sum += (seq_t)29 * cache[i - k];
            }
            sum -= (seq_t)435 * cache[i - 13];
            cache[i] = sum;
        }
    }
    cache_size = n + 1;
}

seq_t a(idx_t n)
{
    extend_cache(n);
    return cache[n];
}
