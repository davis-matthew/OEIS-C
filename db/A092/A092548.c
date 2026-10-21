/*
 * A092548  Molien series for complete weight enumerators of self-dual codes
 *          over GF(4) + GF(4)u with u^2 = 0.
 * https://oeis.org/A092548
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in u1/u2, u1 = 1 + x + 4x² + 3x³ +
 *            53x⁴ + 104x⁵ + … + 3x⁴², u2 = (1‑x²)⁵ (1‑x³) (1‑x⁴)⁶ (1‑x⁶)⁴.
 *            Recurrence: a(n) = pₙ – Σ_{k=1}^{61} qₖ · a(n‑k) (a(m)=0 for
 *            m<0), where pₙ are the coefficients of u1 (zero for n≥43) and qₖ
 *            are the coefficients of u2 (q₀=1). The recurrenc...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* coefficients of u1 (degree 42) -------------------------------------- */
static const long long P[43] = {
    1, 1, 4, 3, 53, 104, 458, 858, 2474, 4839,
    10667, 19018, 34193, 55481, 86078, 125990,
    173466, 230402, 287430, 346462, 393648,
    431930, 450648, 450648, 431930, 393648,
    346462, 287430, 230402, 173466, 125990,
    86078, 55481, 34193, 19018, 10667,
    4839, 2474, 858, 458, 104, 53, 3
};

/* coefficients of u2 – computed once --------------------------------- */
#define U2_DEG 61
static long long Q[U2_DEG + 1];
static int  Q_initialized = 0;

/* multiply current polynomial q (degree *deg) by (1‑xᵐ)ⁿ */
static void multiply_factor(long long *q, int *deg, int n, int m)
{
    for (int r = 0; r < n; ++r) {
        long long tmp[U2_DEG + 1] = {0};
        for (int i = 0; i <= *deg; ++i) {
            if (q[i] == 0) continue;
            tmp[i]     += q[i];        /* *1 */
            tmp[i + m] -= q[i];        /* *‑1 */
        }
        for (int i = 0; i <= *deg + m; ++i) q[i] = tmp[i];
        *deg += m;
    }
}

static void init_Q(void)
{
    if (Q_initialized) return;
    Q_initialized = 1;

    for (int i = 0; i <= U2_DEG; ++i) Q[i] = 0;
    Q[0] = 1;                 /* start with polynomial 1 */
    int deg = 0;

    multiply_factor(Q, &deg, 5, 2);   /* (1‑x²)⁵   */
    multiply_factor(Q, &deg, 1, 3);   /* (1‑x³)     */
    multiply_factor(Q, &deg, 6, 4);   /* (1‑x⁴)⁶   */
    multiply_factor(Q, &deg, 4, 6);   /* (1‑x⁶)⁴   */
    /* deg should now be U2_DEG (61) */
}

/* cache for computed terms ------------------------------------------- */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_size = 0;

static void ensure_cache(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (cache == NULL || need > cache_cap) {
        size_t newcap = cache_cap ? cache_cap * 2 : 128;
        while (need > newcap) newcap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return;          /* out of memory – caller will handle */
        cache = tmp;
        cache_cap = newcap;
    }

    /* compute missing terms */
    for (size_t i = cache_size; i < need; ++i) {
        seq_t term = (i < 43) ? (seq_t)P[i] : (seq_t)0;
        size_t maxk = (i < U2_DEG) ? i : U2_DEG;
        for (size_t k = 1; k <= maxk; ++k) {
            seq_t prod = (seq_t)Q[k] * cache[i - k];
            term -= prod;          /* handles signs automatically */
        }
        cache[i] = term;
    }
    cache_size = need;
}

seq_t a(idx_t n)
{
    init_Q();
    ensure_cache(n);
    return cache[(size_t)n];
}
