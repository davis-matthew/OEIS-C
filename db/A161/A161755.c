/*
 * A161755  Number of reduced words of length n in the Weyl group B_10.
 * https://oeis.org/A161755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   a(n) is the coefficient of x^n in ∏_{k=1..10} (1 + x + x^2 + … +
 *            x^{2k-1}) (degree 100)
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define MAX_N 100
#define M 10

static bool initialized = false;
static seq_t cache[MAX_N + 1];

static void init_cache(void) {
    /* temp holds the cumulative coefficients as 64‑bit signed */
    int64_t coeff[MAX_N + 1] = {0};
    int64_t next[MAX_N + 1] = {0};
    coeff[0] = 1;
    int cur_deg = 0;          /* maximum index currently used */

    for (int k = 1; k <= M; ++k) {
        int len = 2 * k;      /* number of terms in the kth factor ( 1 … 2k-1 ) */
        memset(next, 0, sizeof(next));

        /* convolution: for each existing coefficient add it to all exponents
           of the new factor */
        for (int i = 0; i <= cur_deg; ++i) {
            int64_t val = coeff[i];
            if (!val) continue;
            for (int j = 0; j < len; ++j) {
                next[i + j] += val;
            }
        }
        /* update current coefficients and degree */
        memcpy(coeff, next, sizeof(coeff));
        cur_deg += len - 1;
        if (cur_deg > MAX_N) cur_deg = MAX_N;
    }

    /* Store results in cache, casting to seq_t. All values are non‑negative. */
    for (int n = 0; n <= MAX_N; ++n) {
        cache[n] = (seq_t)coeff[n];
    }
    initialized = true;
}

seq_t a(idx_t n) {
    if (!initialized) init_cache();
    if (n > MAX_N) return 0;
    return cache[n];
}
