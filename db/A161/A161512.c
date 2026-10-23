/*
 * A161512  Number of reduced words of length n in the Weyl group A_19.
 * https://oeis.org/A161512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of x^n in ∏_{k=1}^{19} (1 + x + … + x^k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 17, u64 n <= 190, u128 n <= 190,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The product has degree Σ_{k=1}^{19}k = 190, so a(n)=0 for n>190. */
#include <stdbool.h>

#define MAX_DEG 190

static seq_t poly[MAX_DEG + 1];      /* poly[i] = a(i) for 0 ≤ i ≤ 190 */
static bool poly_ready = false;     /* true after init_poly() */

/* Compute the polynomial once.  All operations are on non‑negative
   integers, so unsigned types are safe. */
static void init_poly(void) {
    for (idx_t i = 0; i <= MAX_DEG; ++i)
        poly[i] = (seq_t)0;
    poly[0] = (seq_t)1;                 /* start with 1 */

    idx_t current_deg = 0;              /* degree of current polynomial */

    for (idx_t k = 1; k <= 19; ++k) {
        idx_t new_deg = current_deg + k;
        seq_t tmp[MAX_DEG + 1] = {0};

        for (idx_t i = 0; i <= current_deg; ++i) {
            seq_t val = poly[i];
            if (val == 0) continue;
            for (idx_t j = 0; j <= k; ++j)
                tmp[i + j] = tmp[i + j] + val;
        }

        for (idx_t i = 0; i <= new_deg; ++i)
            poly[i] = tmp[i];

        current_deg = new_deg;
    }

    poly_ready = true;
}

seq_t a(idx_t n) {
    if (n > MAX_DEG) return (seq_t)0;

    if (!poly_ready) init_poly();
    return poly[n];
}
