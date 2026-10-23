/*
 * A132976  McKay-Thompson series of class 36B for the Monster group with a(0)
 *          = -1.
 * https://oeis.org/A132976
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is the coefficient of q^(n+1) in the product ∏_{m ≥ 1, c_m
 *            = 1} (1 - q^m), where c_m = 1 + [4|m] + [18|m] - [2|m] - [9|m] -
 *            [36|m]. The sequence is the Euler transform of the period‑36
 *            integer sequence
 *            [-1,0,-1,-1,-1,0,-1,-1,0,0,-1,-1,-1,0,-1,-1,-1,0,-1,-1,-1,0,-1,-1,
 *            -1,0,0,-1,-1,0,-1,-1,-1...
 * Range:     i8 n <= 163, i16 n <= 469, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   has_factor, seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_alloc_array.h"  /* seq_alloc_array */
#include "../utils/has_factor.h"

static seq_t *coeff = NULL;      /* coefficients of the product */
static size_t coeff_len = 0;     /* current size of coeff array */
static bool  initialized = false;

/* Pre‑compute the product up to the required maximum target. */
static void precompute(idx_t target)
{
    /* target = n + 1 for the largest requested n */
    idx_t max_target = target;               /* highest m we need to process   */
    size_t new_len   = (size_t)max_target + 1; /* indices 0 … max_target */
    coeff = seq_alloc_array(new_len);
    coeff_len = new_len;

    /* initialise */
    for (size_t i = 0; i < coeff_len; ++i)
        coeff[i] = 0;
    coeff[0] = 1;  /* product starts with 1 */

    /* multiply all required factors */
    for (idx_t m = 1; m <= max_target; ++m) {
        if (!has_factor(m))
            continue;
        for (idx_t i = coeff_len - 1; i >= m; --i) {
            coeff[i] -= coeff[i - m];
        }
    }
}

/* --- Sequence function ---------------------------------------- */
seq_t a(idx_t n)
{
    if (n == -1) return 1;   /* a(-1) = 1 as per offset */

    if (!initialized) {
        /* choose a safe upper bound; 10010 is enough for tests up to 10000 */
        const idx_t MAX_N   = 10000;          /* maximal n expected */
        const idx_t MAX_TAR = MAX_N + 1;      /* n+1 for the largest n */
        precompute(MAX_TAR);
        initialized = true;
    }

    if (n < -1) return 0; /* behaviour below offset is unspecified */

    idx_t target = n + 1;
    if ((size_t)target >= coeff_len) {
        /* requested n larger than pre‑computed range: extend array */
        precompute(target);
        initialized = true;
    }
    return coeff[(size_t)target];
}
