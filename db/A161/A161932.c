/*
 * A161932  Number of reduced words of length n in the Weyl group B_25.
 * https://oeis.org/A161932
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of ∏_{k=1}^{25} (1 + x + … + x^{2k-1})
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 13, u64 n <= 53, u128 n <= 625,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#define M          25
#define MAX_DEG   (M * M)      /* 25*25 = 625 */

static seq_t poly[MAX_DEG + 1];
static bool    initialized = false;
static idx_t   max_deg = 0;

seq_t a(idx_t n)
{
    if (!initialized) {
        /* initialise base polynomial: 1 */
        for (size_t i = 0; i <= MAX_DEG; ++i) poly[i] = (seq_t)0;
        poly[0] = (seq_t)1;
        max_deg = 0;

        for (idx_t k = 1; k <= M; ++k) {
            idx_t add_len = 2 * k;              /* terms in factor k */
            idx_t new_deg = max_deg + add_len - 1;
            seq_t buffer[MAX_DEG + 1];

            /* zero buffer up to new_deg */
            for (size_t i = 0; i <= new_deg; ++i)
                buffer[i] = (seq_t)0;

            /* convolution */
            for (idx_t i = 0; i <= max_deg; ++i) {
                seq_t val = poly[i];
                if (!val) continue;
                for (idx_t j = 0; j < add_len; ++j)
                    buffer[i + j] += val;
            }

            /* copy back to poly */
            for (idx_t i = 0; i <= new_deg; ++i)
                poly[i] = buffer[i];

            max_deg = new_deg;
        }
        initialized = true;
    }

    if (n > max_deg) return (seq_t)0;
    return poly[(size_t)n];
}
