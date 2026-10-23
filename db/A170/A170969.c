/*
 * A170969  Expansion of Product_{i=0..m-1} (1 + x^(4*i+1)) for m = 6.
 * https://oeis.org/A170969
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Sequence: A170969
   Formula: a(n) = number of subsets of {1,5,9,13,17,21} whose sum is n
   (i.e. coefficient of x^n in ∏_{i=0}^{5} (1 + x^(4*i+1)) ) */

seq_t a(idx_t n)
{
    /* The maximum possible sum is 1+5+9+13+17+21 = 66 */
    static seq_t lookup[67];
    static bool initialized = false;

    if (!initialized) {
        const idx_t exps[6] = {1, 5, 9, 13, 17, 21};
        for (idx_t i = 0; i < 67; ++i)
            lookup[i] = 0;

        for (idx_t mask = 0; mask < 64; ++mask) {
            seq_t sum = 0;
            for (idx_t j = 0; j < 6; ++j)
                if (mask & (idx_t)1 << j)
                    sum += (seq_t)exps[j];
            lookup[(idx_t)sum] += 1;
        }
        initialized = true;
    }

    if (n < 67)
        return lookup[n];
    return 0;
}
