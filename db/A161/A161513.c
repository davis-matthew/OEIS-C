/*
 * A161513  Number of reduced words of length n in the Weyl group A_20.
 * https://oeis.org/A161513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 16, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A161513 – number of reduced words of length n in Weyl group A20
   Formula: a(n) = coefficient of x^n in ∏_{k=1}^{20} (1 + x + … + x^k)
   The product is a polynomial of degree 210.  For n > 210, a(n) = 0. */
#include <stdbool.h>

/*   Compile‑time constants.   */
#define MAX_M     20u
#define MAX_DEG   210u      /* 20*21/2 */

/*   Static storage for the coefficients.   */
static seq_t poly[MAX_DEG + 1];
static bool initialized = false;

/*   One‑time initialization of the polynomial coefficients.   */
static void init_poly(void)
{
    if (initialized) return;
    initialized = true;

    poly[0] = 1;                      /* start with the constant 1   */
    idx_t current_deg = 0;            /* highest degree so far       */

    for (idx_t k = 1; k <= MAX_M; ++k) {
        /* For each term (1 + x + … + x^k) we add the existing
           coefficients shifted by 1 … k.  We loop from high to low
           to avoid using updated values in the same iteration.   */
        for (idx_t i = current_deg + 1; i-- > 0;) {
            seq_t val = poly[i];
            for (idx_t j = 1; j <= k; ++j) {
                poly[i + j] += val;
            }
        }
        current_deg += k;           /* degree increases by k       */
    }
}

/*   Public function required by the framework.   */
seq_t a(idx_t n)
{
    init_poly();
    return (n > MAX_DEG) ? (seq_t)0 : poly[n];
}
