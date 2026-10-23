/*
 * A161970  McKay-Thompson series of class 28C for the Monster group with a(0)
 *          = -1.
 * https://oeis.org/A161970
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is the coefficient of q^n in
 *            eta(q)*eta(q^7)/(eta(q^4)*eta(q^28)), i.e. the product ∏_{m≥1}
 *            (1 - q^m)(1 - q^(7m))/((1 - q^(4m))(1 - q^(28m))). For technical
 *            reasons we compute the corresponding series for G(q)=∑_{k≥0} g_k
 *            q^k = eta(q)*eta(q^7)/(eta(q^4)*eta(q^28))*q, then return
 *            a(n)=g_{n+1}...
 * Range:     i8 n <= 54, i16 n <= 162, i32 n <= 518, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* ==================== C implementation ==================== */
#ifndef __cplusplus
static seq_t *cache = NULL;
static idx_t cache_max = -2;   /* largest n for which cache is valid */

seq_t a(idx_t n)
{
    if (n < -1) return (seq_t)0;   /* undefined below offset */

    /* need to rebuild if cache insufficient */
    if (cache == NULL || n > cache_max) {
        idx_t max_n = n;
        size_t len   = (size_t)max_n + 2;   /* indices 0 .. max_n+1 */

        /* allocate main array and temporary buffer */
        seq_t *g   = (seq_t*)calloc(len, sizeof(seq_t));
        seq_t *tmp = (seq_t*)calloc(len, sizeof(seq_t));
        if (!g || !tmp) { free(g); free(tmp); return (seq_t)0; }

        g[0] = (seq_t)1;   /* g_0 = 1 */

        /* main product loop */
        for (idx_t m = 1; m <= max_n + 1; ++m) {
            /* multiply by (1 - q^m) */
            for (idx_t i = (idx_t)(len - 1); i >= (idx_t)m; --i)
                g[i] -= g[i - m];

            /* multiply by (1 - q^{7m}) */
            idx_t k = 7 * m;
            if (k <= (idx_t)(len - 1))
                for (idx_t i = (idx_t)(len - 1); i >= k; --i)
                    g[i] -= g[i - k];

            /* multiply by 1/(1 - q^{4m}) */
            k = 4 * m;
            if (k <= (idx_t)(len - 1)) {
                for (idx_t i = 0; i < len; ++i) {
                    tmp[i] = g[i];
                    if (i >= k) tmp[i] += tmp[i - k];
                }
                for (idx_t i = 0; i < len; ++i) g[i] = tmp[i];
            }

            /* multiply by 1/(1 - q^{28m}) */
            k = 28 * m;
            if (k <= (idx_t)(len - 1)) {
                for (idx_t i = 0; i < len; ++i) {
                    tmp[i] = g[i];
                    if (i >= k) tmp[i] += tmp[i - k];
                }
                for (idx_t i = 0; i < len; ++i) g[i] = tmp[i];
            }
        }

        /* recycle old cache */
        free(cache);
        free(tmp);

        cache     = g;
        cache_max = max_n;
    }

    return cache[(size_t)(n + 1)];
}
#endif /* __cplusplus */

/* ==================== C++ implementation ==================== */
#ifdef __cplusplus
static std::vector<seq_t> cache_vec;
static idx_t cache_max = -2;   /* largest n for which cache is valid */

seq_t a(idx_t n)
{
    if (n < -1) return (seq_t)0;   /* undefined below offset */

    if (cache_vec.empty() || n > cache_max) {
        idx_t max_n = n;
        const size_t len = (size_t)max_n + 2;

        cache_vec.assign(len, (seq_t)0);
        cache_vec[0] = (seq_t)1;   /* g_0 = 1 */

        /* main product loop */
        for (idx_t m = 1; m <= max_n + 1; ++m) {
            /* multiply by (1 - q^m) */
            for (size_t i = len - 1; i >= (size_t)m; --i)
                cache_vec[i] -= cache_vec[i - m];

            /* multiply by (1 - q^{7m}) */
            size_t k = 7 * m;
            if (k <= len - 1)
                for (size_t i = len - 1; i >= k; --i)
                    cache_vec[i] -= cache_vec[i - k];

            /* multiply by 1/(1 - q^{4m}) */
            k = 4 * m;
            if (k <= len - 1) {
                std::vector<seq_t> tmp(len);
                for (size_t i = 0; i < len; ++i) {
                    tmp[i] = cache_vec[i];
                    if (i >= k) tmp[i] += tmp[i - k];
                }
                cache_vec.swap(tmp);
            }

            /* multiply by 1/(1 - q^{28m}) */
            k = 28 * m;
            if (k <= len - 1) {
                std::vector<seq_t> tmp(len);
                for (size_t i = 0; i < len; ++i) {
                    tmp[i] = cache_vec[i];
                    if (i >= k) tmp[i] += tmp[i - k];
                }
                cache_vec.swap(tmp);
            }
        }

        cache_max = max_n;
    }

    return cache_vec[(size_t)(n + 1)];
}
#endif /* __cplusplus */
