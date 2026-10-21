/*
 * A214386  Number of 4X4X4 triangular 0..n arrays with no element lying
 *          outside the (possibly reversed) range delimited by its sw and se
 *          neighbors, and no element equal to its horizontal neighbors.
 * https://oeis.org/A214386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is computed by Newton forward difference interpolation *
 *            from the first 11 terms of the sequence. For m = n-1 * a(n) =
 *            Σ_{k=0}^{10} Δ^k (a(1)) * binom(m, k), * where Δ^k (a(1))
 *            denotes the k‑th forward difference at index 0. * The Δ^k values
 *            are: * Δ^0 = 16 * Δ^1 = 874 * Δ^2 = 9284 * Δ^3 = 4...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 16, u64 n <= 163, u128 n <= 163,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* seq_t and idx_t are provided by the build system. */

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* offset starts at 1 */

    static bool init = false;
    static seq_t delta[11];

    if (!init) {
        /* Initial values a(1) .. a(11) */
        const seq_t init_vals[11] = {
            16ULL,
            890ULL,
            11048ULL,
            74260ULL,
            350232ULL,
            1305392ULL,
            4107248ULL,
            11363940ULL,
            28412824ULL,
            65439858ULL,
            140806952ULL
        };
        seq_t diff[11];
        for (int i = 0; i < 11; ++i) diff[i] = init_vals[i];
        for (int k = 0; k < 11; ++k) {
            delta[k] = diff[0];
            for (int i = 0; i < 10 - k; ++i) {
                diff[i] = diff[i + 1] - diff[i];
            }
        }
        init = true;
    }

    idx_t m = n - 1;          /* shift index: m = n-1 */
    seq_t result = 0;

    for (int k = 0; k < 11; ++k) {
        if ((idx_t)k > m) break;   /* binom(m, k) = 0 for k > m */

        /* Compute binom(m, k) = m! / (k!(m-k)!) using multiplicative formula */
        seq_t bin = 1;
        idx_t start = m - (idx_t)k + 1;   /* first numerator factor */
        for (idx_t i = 1; i <= (idx_t)k; ++i) {
            bin = bin * (start + i - 1) / i;
        }

        result += delta[k] * bin;
    }

    return result;
}
