/*
 * A170971  Expansion of Product_{i=0..m-1} (1 + x^(4*i+1)) for m = 8.
 * https://oeis.org/A170971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = number of subsets of {1,5,9,13,17,21,25,29} whose sum is
 *            n (equivalently, the coefficient of x^n in ∏_{i=0}^7 (1 +
 *            x^{4*i+1}) )
 * Range:     u8 n <= 120, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

extern seq_t a(idx_t n);   /* Declaration for visibility if needed */

seq_t a(idx_t n)
{
    static seq_t cache[121];
    static bool init = false;
    const idx_t S[8] = {1,5,9,13,17,21,25,29};

    if (!init) {
        seq_t local[121];
        for (idx_t i = 0; i <= 120; ++i)
            local[i] = 0;

        for (idx_t mask = 0; mask < (1u << 8); ++mask) {
            idx_t sum = 0;
            for (idx_t j = 0; j < 8; ++j)
                if (mask & (1u << j))
                    sum += S[j];
            /* sum is always <= 120 for this set */
            local[sum] += 1;
        }

        for (idx_t i = 0; i <= 120; ++i)
            cache[i] = local[i];
        init = true;
    }

    if (n > 120)
        return 0;   /* coefficients outside the polynomial range are zero */

    return cache[n];
}
