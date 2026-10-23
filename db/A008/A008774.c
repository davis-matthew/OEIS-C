/*
 * A008774  Theta series of (probably nonexistent) exceptionally good
 *          16-dimensional sphere packing.
 * https://oeis.org/A008774
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (E4(q) * 2 * (E4(q^2) - E4(q^4)) + E4(q^2) * (32 *
 *            E4(q^4) - 17 * E4(q^2)) ) / 15
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 20, u64 n <= 472, u128 n <= 1000,
 *            big any n
 * Helpers:   compute_sigma3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where E4(q) = 1 + 240 * Σ_{m≥1} σ₃(m) q^m  and σ₃(m)=Σ_{d|m} d³ */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/compute_sigma3.h"

static seq_t *cache = NULL;     /* cached a(0)…a(cache_size-1) */
static idx_t cache_size = 0;    /* number of cached terms */

/* main sequence function */
seq_t a(idx_t n)
{
    if (n >= cache_size) {
        /* allocate new cache */
        size_t N = (size_t)n;
        seq_t *new_cache = (seq_t *)calloc(N + 1, sizeof(seq_t));
        if (!new_cache) return 0;   /* out of memory – return 0 */

        /* compute divisor sums (sigma3) up to N */
        unsigned long long *sigma3 = compute_sigma3(N);
        if (!sigma3) { free(new_cache); return 0; }

        /* build coefficient arrays for E4(q), E4(q²), E4(q⁴) */
        seq_t *A0 = (seq_t *)calloc(N + 1, sizeof(seq_t));
        seq_t *A2 = (seq_t *)calloc(N + 1, sizeof(seq_t));
        seq_t *A4 = (seq_t *)calloc(N + 1, sizeof(seq_t));
        if (!A0 || !A2 || !A4) { free(new_cache); free(sigma3); return 0; }

        A0[0] = 1;
        for (size_t i = 1; i <= N; ++i)
            A0[i] = (seq_t)(240ULL * sigma3[i]);

        A2[0] = 1;
        for (size_t i = 2; i <= N; i += 2)
            A2[i] = (seq_t)(240ULL * sigma3[i / 2]);

        A4[0] = 1;
        for (size_t i = 4; i <= N; i += 4)
            A4[i] = (seq_t)(240ULL * sigma3[i / 4]);

        /* temporary array to accumulate the numerator before division by 15 */
        __int128 *tmp = (__int128 *)calloc(N + 1, sizeof(__int128));
        if (!tmp) { free(new_cache); free(sigma3); free(A0); free(A2); free(A4); return 0; }

        /* single double loop accumulating all required convolutions */
        for (size_t i = 0; i <= N; ++i) {
            seq_t a0i = A0[i];
            seq_t a2i = A2[i];
            seq_t a4i = A4[i];
            if (a0i == 0 && a2i == 0 && a4i == 0) continue;   /* skip zero rows */

            for (size_t j = 0; j <= N - i; ++j) {
                size_t t = i + j;
                seq_t a0j = A0[j];
                seq_t a2j = A2[j];
                seq_t a4j = A4[j];

                /* 2 * E0 * E2 */
                tmp[t] += (__int128)2 * (__int128)a0i * (__int128)a2j;
                /* -2 * E0 * E4 */
                tmp[t] -= (__int128)2 * (__int128)a0i * (__int128)a4j;
                /* +32 * E2 * E4 */
                tmp[t] += (__int128)32 * (__int128)a2i * (__int128)a4j;
                /* -17 * E2 * E2 */
                tmp[t] -= (__int128)17 * (__int128)a2i * (__int128)a2j;
            }
        }

        /* finalize cache values by dividing by 15 */
        for (size_t i = 0; i <= N; ++i)
            new_cache[i] = (seq_t)(tmp[i] / 15);

        /* free temporary arrays */
        free(tmp);
        free(A0);
        free(A2);
        free(A4);
        free(sigma3);

        /* replace old cache */
        if (cache) free(cache);
        cache = new_cache;
        cache_size = N + 1;
    }
    return cache[n];
}
