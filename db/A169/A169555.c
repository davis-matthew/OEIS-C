/*
 * A169555  Number of reduced words of length n in Coxeter group on 14
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^35 = I.
 * https://oeis.org/A169555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by the generating function (t^35 + 2*t^34 + … +
 *            2*t + 1) / (78*t^35 – 12*t^34 – … – 12*t + 1). Sequence
 *            satisfies the linear recurrence for n ≥ 35: a(n) =
 *            12·∑_{k=1}^{34} a(n–k) – 78·a(n–35) and for 0 ≤ n < 35 a(n) =
 *            coeff(n) + 12·∑_{k=1}^{n} a(n–k), coeff(0)=1, coeff(n)=2 for 1
 *            ≤...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* static cache for computed terms */
static seq_t *cache   = NULL;   /* pointer to array of seq_t */
static size_t cache_len = 0;    /* number of terms currently stored   */
static size_t cache_cap = 0;    /* capacity of the array              */

/* increase capacity to at least 'needed' */
static void ensure_capacity(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t newcap = cache_cap > 0 ? cache_cap : 16;
    while (newcap < needed) newcap <<= 1;
    seq_t *newbuf = (seq_t*)malloc(newcap * sizeof(seq_t));
    if (!newbuf) { abort(); }
    if (cache) {
        for (size_t i = 0; i < cache_len; ++i) newbuf[i] = cache[i];
        free(cache);
    }
    cache = newbuf;
    cache_cap = newcap;
}

/* compute terms up to index n (inclusive), storing them in cache */
static void compute_until(idx_t n)
{
    if (cache_len > (size_t)n) return;          /* already computed     */
    ensure_capacity((size_t)(n + 1));

    /* start from the first missing index */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t term = 0;
        if (i < 35) {                           /* early terms */
            seq_t coeff = (i == 0) ? 1 : 2;
            seq_t sum   = 0;
            for (idx_t k = 1; k <= i; ++k) {
                sum += cache[i - k] * 12;
            }
            term = coeff + sum;
        } else if (i == 35) {                   /* special 35th term */
            seq_t sum = 0;
            for (idx_t k = 1; k <= 34; ++k) {
                sum += cache[i - k] * 12;
            }
            term = 1 + sum - cache[0] * 78;
        } else {                                /* recurrence for n ≥ 36 */
            seq_t sum = 0;
            for (idx_t k = 1; k <= 34; ++k) {
                sum += cache[i - k] * 12;
            }
            term = sum - cache[i - 35] * 78;
        }
        cache[i] = term;
    }
    cache_len = (size_t)(n + 1);
}

/* external linkage: seq_t a(idx_t n) */
seq_t a(idx_t n)
{
    compute_until(n);
    return cache[(size_t)n];
}
