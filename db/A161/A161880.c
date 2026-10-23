/*
 * A161880  Number of reduced words of length n in the Weyl group B_20.
 * https://oeis.org/A161880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = coefficient of x^n in ∏_{k=1}^{20} (1 - x^{2k})/(1 - x)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 16, u64 n <= 85, u128 n <= 400,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This polynomial expands to ∏_{k=1}^{20} (1 + x + … + x^{2k-1}) */
/* and has non‑zero coefficients only for 0 ≤ n ≤ 400. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

static seq_t *coeff_cache = NULL;          /* size 401 */
static bool coeff_init = false;
static const idx_t MAX_N = 400;

static void init_coefficients(void) {
    if (coeff_init) return;
    coeff_init = true;
    coeff_cache = (seq_t *)calloc(MAX_N + 1, sizeof(seq_t));
    if (!coeff_cache) return; /* allocation failed – fallback to zero */
    coeff_cache[0] = 1;        /* start with polynomial 1 */
    /* Convolve sequentially with 1 + x + … + x^{2k-1} for k = 1..20 */
    for (idx_t k = 1; k <= 20; ++k) {
        seq_t *next = (seq_t *)calloc(MAX_N + 1, sizeof(seq_t));
        if (!next) { /* allocation failed – keep current coefficients */
            free(coeff_cache);
            coeff_cache = NULL;
            coeff_init = false;
            return;
        }
        idx_t len = 2 * k - 1;              /* number of terms in factor */
        for (idx_t j = 0; j <= MAX_N; ++j) {
            seq_t sum = 0;
            for (idx_t i = 0; i <= len && j >= i; ++i) {
                sum += coeff_cache[j - i];
            }
            next[j] = sum;
        }
        free(coeff_cache);
        coeff_cache = next;
    }
}

seq_t a(idx_t n) {
    if (!coeff_init) init_coefficients();
    if (!coeff_cache) return 0; /* bail out if allocation failed */
    if (n <= MAX_N) return coeff_cache[n];
    return 0;
}
