/*
 * A161477  Number of reduced words of length n in the Weyl group A_16.
 * https://oeis.org/A161477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of x^n in ∏_{k=1}^{16} (1 + x + … + x^k)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 22, u64 n <= 136, u128 n <= 136,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static seq_t a_coeff[137];          /* a_coeff[n] holds a(n) for 0 ≤ n ≤ 136 */
static int  a_initialized = 0;      /* 0 → we haven't computed the polynomial yet */

/* Compute the coefficients once. */
static void init_a_coeff(void)
{
    if (a_initialized)
        return;

    /* Temporary arrays for polynomial multiplication.
       Temporary arrays are kept static to avoid repeated allocation. */
    static seq_t curr[137];
    static seq_t next[137];

    /* Start with 1 (constant term). */
    for (int i = 0; i < 137; ++i)
        curr[i] = (seq_t)0;
    curr[0] = (seq_t)1;

    int current_deg = 0;   /* degree of the current polynomial */

    /* Sequentially multiply by (1 + x + … + x^k) for k = 1 … 16. */
    for (int k = 1; k <= 16; ++k) {
        int new_deg = current_deg + k;
        /* Zero the next array up to new_deg. */
        for (int i = 0; i <= new_deg; ++i)
            next[i] = (seq_t)0;

        for (int i = 0; i <= current_deg; ++i) {
            seq_t v = curr[i];
            for (int j = 0; j <= k; ++j) {
                next[i + j] = next[i + j] + v;
            }
        }

        /* Copy back into curr for the next iteration. */
        for (int i = 0; i <= new_deg; ++i)
            curr[i] = next[i];
        current_deg = new_deg;
    }

    /* Store the computed coefficients in the public array. */
    for (int i = 0; i <= 136; ++i)
        a_coeff[i] = curr[i];

    a_initialized = 1;
}

/* External sequence function. */
seq_t a(idx_t n)
{
    init_a_coeff();
    if (n > 136)
        return (seq_t)0;                  /* coefficient is zero beyond degree 136 */
    return a_coeff[(size_t)n];
}
