/*
 * A051562  Second unsigned column of triangle A051380.
 * https://oeis.org/A051562
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n! * Σ_{k=0}^{n-1} binom(8+k,8) / (n-k) (n ≥ 1) a(0) = 0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *fact_cache = NULL;   /* fact_cache[i] = i! : i from 0 .. */
static size_t fact_len = 0;        /* allocated length of fact_cache */

/* Ensure the factorial cache is ready for indices 0..n */
static void ensure_fact_cache(idx_t n)
{
    if (fact_len <= n) {
        size_t new_len = n + 1;
        seq_t *p = (seq_t *)realloc(fact_cache, new_len * sizeof(seq_t));
        if (!p) abort();          /* allocation failure --> terminate */
        fact_cache = p;
        /* compute new entries from the current length */
        for (size_t i = fact_len; i <= (size_t)n; ++i) {
            if (i == 0 || i == 1) {
                fact_cache[i] = (seq_t)1;
            } else {
                fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
            }
        }
        fact_len = new_len;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* a(0) = 0 */

    /* make sure factorials up to n are ready */
    ensure_fact_cache(n);

    seq_t fact_n = fact_cache[n];

    /* pre = n! / (n - k)  with k=0 at first iteration */
    seq_t pre = fact_n / (seq_t)n;
    /* binomial(8 + k, 8)  with k=0 => 1 */
    seq_t binom = (seq_t)1;
    seq_t sum = (seq_t)0;

    for (idx_t k = 0; k < n; ++k) {
        /* term_k = binom * pre */
        sum += binom * pre;

        /* update for next k */
        if (k + 1 < n) {
            /* pre = pre * (n - k) / (n - k - 1)   (n-k-1 ≥ 1) */
            pre = pre * (seq_t)(n - k) / (seq_t)(n - k - 1);

            /* binom = binom * (8 + k + 1) / (k + 1) */
            binom = binom * (seq_t)(8 + k + 1) / (seq_t)(k + 1);
        }
    }
    return sum;
}
