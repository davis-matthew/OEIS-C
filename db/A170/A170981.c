/*
 * A170981  Expansion of (Product_{i=1..m-1} (1 + x^(4*i-1))) * (1 +
 *          x^(2*m-1)) for m = 9.
 * https://oeis.org/A170981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = coefficient of x^n in (∏_{i=1..8} (1 + x^(4*i-1))) * (1 +
 *            x^17)
 * Range:     u8 n <= 153, u16 n <= 153, u32 n <= 153, u64 n <= 153,
 *            u128 n <= 153, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A170981 */

#include <stdbool.h>

static const idx_t A170981_MAX_INDEX = 153; /* highest exponent */

static seq_t a_cache[154]; /* indices 0..153 */
static bool a_cache_computed = false;

seq_t a(idx_t n)
{
    if (n > A170981_MAX_INDEX)
        return 0;

    if (!a_cache_computed) {
        /* initialise */
        for (idx_t i = 0; i <= A170981_MAX_INDEX; ++i)
            a_cache[i] = 0;
        a_cache[0] = 1;

        /* exponents present in the product */
        const idx_t exps[9] = {3, 7, 11, 15, 19, 23, 27, 31, 17};

        /* subset-sum dynamic programming */
        for (int ei = 0; ei < 9; ++ei) {
            idx_t e = exps[ei];
            for (idx_t i = A170981_MAX_INDEX; i >= e; --i)
                a_cache[i] = a_cache[i] + a_cache[i - e];
        }

        a_cache_computed = true;
    }

    return a_cache[n];
}
