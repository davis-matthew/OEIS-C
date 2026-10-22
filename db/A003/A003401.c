/*
 * A003401  Numbers of edges of regular polygons constructible with ruler (or,
 *          more precisely, an unmarked straightedge) and compass.
 * https://oeis.org/A003401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = n-th smallest number of the form 2^k * (product of
 *            distinct Fermat primes 3,5,17,257,65537). Computed block by
 *            block: the terms in [2^k, 2^(k+1)) are d*2^(k-L+1), one for each
 *            odd part d of bit length L <= k+1, ordered by the mantissa
 *            d/2^(L-1)
 * Range:     u8 n <= 36, u16 n <= 136, u32 n <= 528, u64 n <= 1552,
 *            u128 n <= 3600, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A3401_d[32];
static int A3401_L[32];
static bool A3401_init = false;

static void A3401_setup(void) {
    const uint64_t fermat[5] = {3, 5, 17, 257, 65537};
    int i, j, mask;
    for (mask = 0; mask < 32; mask++) {
        uint64_t p = 1;
        int L = 0;
        for (i = 0; i < 5; i++)
            if (mask & (1 << i)) p *= fermat[i];
        A3401_d[mask] = p;
        while ((p >> L) != 0) L++;
        A3401_L[mask] = L;
    }
    /* sort by mantissa d / 2^(L-1): compare d * 2^(32-L) */
    for (i = 1; i < 32; i++) {
        uint64_t d = A3401_d[i];
        int L = A3401_L[i];
        uint64_t key = d << (32 - L);
        j = i - 1;
        while (j >= 0 && (A3401_d[j] << (32 - A3401_L[j])) > key) {
            A3401_d[j + 1] = A3401_d[j];
            A3401_L[j + 1] = A3401_L[j];
            j--;
        }
        A3401_d[j + 1] = d;
        A3401_L[j + 1] = L;
    }
    A3401_init = true;
}

seq_t a(idx_t n) {
    idx_t remaining, k;
    int i;
    if (!A3401_init) A3401_setup();
    remaining = n; /* 1-based rank still to skip */
    for (k = 0;; k++) {
        idx_t cnt = 0;
        for (i = 0; i < 32; i++)
            if ((idx_t)A3401_L[i] <= k + 1) cnt++;
        if (remaining > cnt) {
            remaining -= cnt;
            continue;
        }
        for (i = 0; i < 32; i++) {
            if ((idx_t)A3401_L[i] <= k + 1) {
                remaining--;
                if (remaining == 0) {
                    seq_t v = (seq_t)A3401_d[i];
                    idx_t sh = k + 1 - (idx_t)A3401_L[i];
                    idx_t t;
                    for (t = 0; t < sh; t++) v = v * 2;
                    return v;
                }
            }
        }
    }
}
