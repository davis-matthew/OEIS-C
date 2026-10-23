/*
 * A162183  Number of reduced words of length n in the Weyl group B_45.
 * https://oeis.org/A162183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = coefficient of x^n in
 *      ∏_{k=1..45} (1 - x^(2k)) / (1 - x).
 * This expands to a polynomial of degree 2025; coefficients beyond
 * that degree are zero.
 *
 * The implementation multiplies the polynomial by each factor
 * (1 + x + … + x^(2k-1)) sequentially, using simple convolution.
 * All arithmetic is performed in the integer type seq_t.
 */

#include <stddef.h>

#define MAX_K 45U
#define MAX_DEG ((MAX_K) * (MAX_K))   /* 45^2 = 2025 */

seq_t a(idx_t n) {
    /* Static tables holding coefficients; initialised on first use. */
    static seq_t table[MAX_DEG + 1] = {0};
    static seq_t cur[MAX_DEG + 1]   = {0};
    static seq_t nxt[MAX_DEG + 1];
    static int initialized = 0;

    if (!initialized) {
        cur[0] = 1;
        idx_t cur_max = 0;

        for (idx_t k = 1; k <= MAX_K; ++k) {
            idx_t stride = 2 * k;        /* window size: 2k */
            idx_t new_max = cur_max + stride - 1;

            /* Zero the workspace up to new_max */
            for (idx_t j = 0; j <= new_max; ++j)
                nxt[j] = 0;

            /* Convolution: add cur[i] to nxt[i + t] for t = 0..stride-1 */
            for (idx_t i = 0; i <= cur_max; ++i) {
                seq_t val = cur[i];
                for (idx_t t = 0; t < stride; ++t)
                    nxt[i + t] += val;
            }

            /* Prepare for next iteration */
            for (idx_t j = 0; j <= new_max; ++j)
                cur[j] = nxt[j];

            cur_max = new_max;
        }

        /* Copy the final polynomial to the cache table. */
        for (idx_t j = 0; j <= MAX_DEG; ++j)
            table[j] = cur[j];

        initialized = 1;
    }

    if (n > MAX_DEG)
        return 0;

    return table[(size_t)n];
}
