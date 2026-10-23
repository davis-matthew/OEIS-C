/*
 * A161653  Number of reduced words of length n in the Weyl group A_40.
 * https://oeis.org/A161653
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k=2}^{41}(1+x+⋯+x^{k-1}) The
 *            product can be rewritten as ∏_{k=1}^{40}(1+x+⋯+x^k), a
 *            polynomial of degree 820; a(n)=0 for n>820
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* maximum degree for the generating polynomial of A_40 */
    const size_t MAX_N = 820;

    /* Cache the precomputed values; lazy initialization. */
    static seq_t *cache = NULL;
    static int initialized = 0;

    if (n > MAX_N)          /* out of range → 0 */
        return 0;

    if (!initialized) {
        /* Allocate and zero the cache array. */
        cache = (seq_t *)malloc((MAX_N + 1) * sizeof(seq_t));
        for (size_t i = 0; i <= MAX_N; ++i)
            cache[i] = 0;
        cache[0] = 1;      /* start with the constant polynomial 1 */

        /* Multiply progressively by (1 + x + ⋯ + x^k) for k=1..40. */
        for (size_t k = 1; k <= 40; ++k) {
            /* Process from high degree to low to avoid overwriting
               values that are still needed. */
            for (int i = (int)MAX_N; i >= 0; --i) {
                seq_t val = cache[i];
                if (val == 0)
                    continue;
                for (size_t add = 1; add <= k && i + add <= MAX_N; ++add)
                    cache[i + add] += val;
            }
        }
        initialized = 1;
    }

    return cache[(size_t)n];
}
