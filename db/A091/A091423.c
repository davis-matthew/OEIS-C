/*
 * A091423  G.f.: ((1 + x^9)*(1 + x^(15)) ) / ( (1 - x^3)*(1 - x^5)*(1 -
 *          x^8)*(1 - x^(12))).
 * https://oeis.org/A091423
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f. = ((1 + x^9)*(1 + x^15)) / ((1 - x^3)*(1 - x^5)*(1 -
 *            x^8)*(1 - x^12))
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: d0*a(n) + Σ_{k=1}^{28} d_k*a(n-k) = n_n,
   where d_k are coefficients of (1-x^3)(1-x^5)(1-x^8)(1-x^12)
   (d_k = ±1) and n_n = 1 if n∈{0,9,15,24} else 0.  d0 = 1.  */
#include <stdlib.h>
#include <stdint.h>

static int8_t denom[29];
static int denom_deg = 0;
static int denom_built = 0;

static void build_denominator(void)
{
    /* start with the polynomial 1 */
    for (int i = 0; i < 29; ++i) denom[i] = 0;
    denom[0] = 1;
    int deg = 0;

    const int ks[4] = {3, 5, 8, 12};
    for (int j = 0; j < 4; ++j) {
        int k = ks[j];
        int new_deg = deg + k;
        int8_t tmp[29];
        for (int i = 0; i <= new_deg; ++i) tmp[i] = 0;
        for (int i = 0; i <= deg; ++i) {
            tmp[i] += denom[i];            /* term with coefficient 1 */
            if (i + k <= 28) {
                tmp[i + k] -= denom[i];   /* term with coefficient -1 */
            }
        }
        for (int i = 0; i <= new_deg; ++i) denom[i] = tmp[i];
        deg = new_deg;
    }
    denom_deg = deg;   /* should be 28 */
    denom_built = 1;
}

static seq_t *cache = NULL;   /* dynamic cache for a(n) */
static size_t cache_cap = 0;   /* current capacity of cache */

seq_t a(idx_t n)
{
    if (!denom_built) build_denominator();

    /* number of terms we already have */
    size_t start = cache_cap;               /* next index to compute */

    /* enlarge cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp && new_cap > 0) return 0;  /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms in ascending order */
    for (idx_t i = (idx_t)start; (size_t)i <= n; ++i) {
        seq_t sum = 0;                          /* Σ d_k * a(i-k) */
        int maxk = (int)i < denom_deg ? (int)i : denom_deg;
        for (int k = 1; k <= maxk; ++k) {
            int8_t dk = denom[k];
            if (dk == 0) continue;
            if (dk < 0)
                sum -= cache[i - k];
            else
                sum += cache[i - k];
        }
        int coeff = (i == 0 || i == 9 || i == 15 || i == 24) ? 1 : 0;
        cache[i] = (seq_t)coeff - sum;   /* a(i) = coeff - Σ d_k*a(i-k) */
    }

    return cache[n];
}
