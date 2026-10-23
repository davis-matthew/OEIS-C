/*
 * A161717  Number of reduced words of length n in the Weyl group B_8.
 * https://oeis.org/A161717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   a(n) = coefficient of x^n in ∏_{k=1..8} (1-x^{2k})/(1-x)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n>64 the value is 0. The function uses lazy initialization. */

#include <stddef.h>
#include <stdbool.h>

/* Array of coefficients for degrees 0..64 */
static seq_t _a161717_coeff[65];
static bool _a161717_initialized = false;

static void _a161717_compute(void)
{
    if (_a161717_initialized) return;

    /* Start with polynomial 1 */
    for (size_t i = 0; i <= 64; ++i) _a161717_coeff[i] = 0;
    _a161717_coeff[0] = 1;

    /* Current highest degree with non‑zero coefficient */
    size_t current_max = 0;

    for (int k = 1; k <= 8; ++k)
    {
        /* Factor (1 + x + ... + x^(2k-1)) has degree 2k-1 */
        size_t step = 2 * (size_t)k - 1;
        size_t new_max = current_max + step;

        seq_t new_coeff[65];
        for (size_t i = 0; i <= new_max; ++i)
            new_coeff[i] = 0;

        for (size_t i = 0; i <= current_max; ++i)
        {
            seq_t val = _a161717_coeff[i];
            for (size_t r = 0; r <= step; ++r)
            {
                new_coeff[i + r] += val;
            }
        }

        /* Copy back */
        for (size_t i = 0; i <= new_max; ++i)
            _a161717_coeff[i] = new_coeff[i];

        current_max = new_max;
    }

    _a161717_initialized = true;
}

seq_t a(idx_t n)
{
    if (n > 64) return 0;
    _a161717_compute();
    return _a161717_coeff[n];
}
