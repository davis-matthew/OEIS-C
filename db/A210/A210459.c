/*
 * A210459  McKay-Thompson series of class 20A for the Monster group with a(0)
 *          = 4.
 * https://oeis.org/A210459
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn
 * Formula:   The McKay–Thompson series A210459 is given by the Euler
 *            transform of a period‑20 sequence c_k = [4, –4, 4, 0, 8, –4, 4,
 *            0, 4, –8, 4, 0, 4, –4, 8, 0, 4, –4, 4, 0]. If g(m) denotes the
 *            coefficient of x^m in ∏_{k≥1}(1+(-x)^k)^{-4}(1+(-x)^{5k})^{-4}
 *            then g(0) = 1, B(n) = ∑_{d|n} d·c_d, g(m) = (1/m)·∑...
 * Range:     u8 n <= 8, u16 n <= 27, u32 n <= 90, u64 n <= 313, u128 n <= 990,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* period‑20 sequence c_k (index starts at 1) */
static const int64_t cyc[20] = {
    4, -4, 4, 0, 8, -4, 4, 0, 4, -8,
   4, 0, 4, -4, 8, 0, 4, -4, 4, 0
};

/* Auxiliary: compute B(k) = sum_{d|k} d·c_d */
static int64_t compute_Bk(size_t k)
{
    int64_t sum = 0;
    for (size_t d = 1; d * d <= k; ++d) {
        if (k % d == 0) {
            int64_t cd = cyc[(d - 1) % 20];
            sum += (int64_t)d * cd;
            size_t e = k / d;
            if (e != d) {
                int64_t ce = cyc[(e - 1) % 20];
                sum += (int64_t)e * ce;
            }
        }
    }
    return sum;
}

/* Seed for a(-1) */
seq_t a(idx_t n)
{
    if (n < -1) return 0;                 /* undefined below offset */

    size_t idx = (size_t)(n + 1);          /* g(idx) -> a(n)   */

    /* ----- caching of coefficients g(m) ----- */
    static seq_t *g = NULL;
    static size_t cap_g = 0, len_g = 0;

    /* ----- caching of B(k) values ----- */
    static int64_t *B = NULL;
    static size_t cap_B = 0, len_B = 0;

    /* initialise g if first call */
    if (cap_g == 0) {
        cap_g = 4;
        g = (seq_t*)malloc(cap_g * sizeof(seq_t));
        if (!g) return 0;
        g[0] = (seq_t)1;      /* a(-1) = 1 */
        len_g = 1;
    }

    /* grow g if necessary */
    if (idx >= cap_g) {
        size_t new_cap = idx + 1;
        if (new_cap < cap_g * 2) new_cap = cap_g * 2;
        seq_t *tmp = (seq_t*)realloc(g, new_cap * sizeof(seq_t));
        if (!tmp) return 0;
        g = tmp;
        cap_g = new_cap;
    }

    /* initialise B array if first call */
    if (cap_B == 0) {
        cap_B = 4;
        B = (int64_t*)malloc(cap_B * sizeof(int64_t));
        if (!B) return 0;
        B[0] = 0;          /* unused */
        len_B = 1;
    }

    /* grow B if necessary */
    if (idx >= cap_B) {
        size_t new_cap = idx + 1;
        if (new_cap < cap_B * 2) new_cap = cap_B * 2;
        int64_t *tmpB = (int64_t*)realloc(B, new_cap * sizeof(int64_t));
        if (!tmpB) return 0;
        B = tmpB;
        cap_B = new_cap;
    }

    /* compute missing B(k) */
    for (size_t k = len_B; k <= idx; ++k) {
        B[k] = compute_Bk(k);
    }
    len_B = idx + 1;

    /* compute missing g(m) using the recurrence */
    for (size_t m = len_g; m <= idx; ++m) {
#ifdef __cplusplus
        seq_t sum = 0;
        for (size_t k = 1; k <= m; ++k)
            sum += g[m - k] * (seq_t)B[k];   /* big‑integer multiplication */
        g[m] = sum / (seq_t)m;               /* exact division */
#else
        __int128 sum = 0;
        for (size_t k = 1; k <= m; ++k)
            sum += (__int128)g[m - k] * (__int128)B[k];
        g[m] = (seq_t)(sum / (__int128)m);   /* exact division */
#endif
    }
    len_g = idx + 1;

    return g[idx];
}
