/*
 * A109974  Array read by downwards antidiagonals: sigma_k(n) for n >= 1, k >=
 *          0.
 * https://oeis.org/A109974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl, nice
 * Formula:   for n ≥ 0 let d = the smallest integer with d*(d+1)/2 > n
 *            (diagonal index) p = n – d*(d-1)/2 (position in that diagonal)
 *            Then a(n) = σ_p(d), the sum of the p‑th powers of the positive
 *            divisors of d
 * Range:     u8 n <= 31, u16 n <= 72, u32 n <= 181, u64 n <= 452,
 *            u128 n <= 1159, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define MAX_D 200               // maximum diagonal needed for n <= 10000
static seq_t sigma[MAX_D + 1][MAX_D];
static bool init_done = false;

static void init_sigma(void) {
    if (init_done) return;
    /* zero the table */
    for (int i = 0; i <= MAX_D; ++i)
        for (int k = 0; k < MAX_D; ++k)
            sigma[i][k] = 0;

    /* fill sigma[n][k] = σ_k(n) for n >= 1, k >= 0 */
    for (int n = 1; n <= MAX_D; ++n) {
        for (int d = 1; d * d <= n; ++d) {
            if (n % d == 0) {
                seq_t pow = 1;
                for (int k = 0; k < MAX_D; ++k) {
                    sigma[n][k] += pow;
                    pow *= d;
                }
                int other = n / d;
                if (other != d) {
                    pow = 1;
                    for (int k = 0; k < MAX_D; ++k) {
                        sigma[n][k] += pow;
                        pow *= other;
                    }
                }
            }
        }
    }
    init_done = true;
}

seq_t a(idx_t n) {
    init_sigma();
    /* find diagonal d such that d*(d+1)/2 > n */
    uint64_t d = 1;
    while ((d * (d + 1) / 2) <= n) ++d;

    uint64_t start = d * (d - 1) / 2;
    uint64_t p = n - start;           // position in diagonal, 0‑based
    uint64_t target_n = d - p;        // the n value in σ_p(n)
    uint64_t k = p;                   // the k value

    return sigma[(int)target_n][(int)k];
}
