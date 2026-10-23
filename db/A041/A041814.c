/*
 * A041814  Numerators of continued fraction convergents to sqrt(428).
 * https://oeis.org/A041814
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = numerator of the n-th convergent of √428 (continued
 *            fraction). Recurrence: p_{-1}=1, p_{-2}=0; p_n = a_n * p_{n-1} +
 *            p_{n-2}, where a_0 = ⌊√428⌋ = 20 and a_n for n≥1 are the
 *            periodic continued‑fraction coefficients of √428
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 16, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const idx_t D   = 428;      /* radicand                     */
static const idx_t A0  = 20;       /* floor(√428)                  */

/* Store the periodic part of the continued fraction of √428.  For this
   radicand the period is short, but we keep a generous limit. */
static idx_t period_terms[200];
static idx_t period_len   = 0;

/* Initialise period_terms[] if it has not been done yet. */
static void init_period(void)
{
    if (period_len != 0)
        return;  /* already initialised */

    idx_t m = 0;
    idx_t d = 1;
    idx_t a = A0;

    do {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (A0 + m) / d;
        period_terms[period_len++] = a;
    } while (a != 2 * A0);
}

/* Return the n-th coefficient a_n of the continued fraction of √428
   (a_0 = A0, a_1 … are stored in period_terms). */
static seq_t coef(idx_t n)
{
    if (n == 0)
        return (seq_t)A0;
    idx_t pos = (n - 1) % period_len;
    return (seq_t)period_terms[pos];
}

/* Cache of previously computed numerators p_n. */
static seq_t *cache     = NULL;
static size_t cache_len = 0;  /* number of terms stored (next index to fill) */
static size_t cache_cap = 0;  /* allocated capacity of cache             */

seq_t a(idx_t n)
{
    init_period();

    /* Grow the cache if needed. */
    while (cache_len <= (size_t)n) {
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap == 0 ? 64 : cache_cap << 1;
#ifdef __cplusplus
            seq_t *tmp = new seq_t[new_cap];
            if (cache) {
                for (size_t i = 0; i < cache_len; ++i)
                    tmp[i] = cache[i];
                delete[] cache;
            }
#else
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                exit(1);
#endif
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute p_cache_len. */
        idx_t k = (idx_t)cache_len;
        seq_t a_k  = coef(k);

        seq_t prev1, prev2;
        if (cache_len == 0) {          /* p_{-1} = 1, p_{-2} = 0 */
            prev1 = (seq_t)1;
            prev2 = (seq_t)0;
        } else if (cache_len == 1) {    /* p_0 known, p_{-1} = 1 */
            prev1 = cache[0];          /* p_0 */
            prev2 = (seq_t)1;          /* p_{-1} */
        } else {
            prev1 = cache[cache_len - 1]; /* p_{n-1} */
            prev2 = cache[cache_len - 2]; /* p_{n-2} */
        }

        seq_t curr = a_k * prev1 + prev2;
        cache[cache_len++] = curr;
    }

    return cache[n];
}
