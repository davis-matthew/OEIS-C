/*
 * A017742  Binomial coefficients C(n,78).
 * https://oeis.org/A017742
 *
 * Offset:    78    (a(78) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n, 78) for n >= 78
 * Range:     u8 n <= 79, u16 n <= 80, u32 n <= 84, u64 n <= 96, u128 n <= 133,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A017742: C(n,78)  (offset 78) */
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 78)
        return 0;                        /* behaviour below the offset is irrelevant */

    /* For small seq_t types compute multiplicatively in a wider type.
       This keeps the intermediate value in bounds whenever a(n)
       fits in seq_t. */
    if (sizeof(seq_t) <= 8) {
        __uint128_t res = 1;
        for (idx_t k = 1; k <= 78; ++k) {
            res = res * (n - 78 + k) / k;
        }
        return (seq_t)res;
    }

    /* For larger seq_t types (128-bit or arbitrary precision) use the
       provided binomial implementation which handles large numbers. */
    return binomial(n, 78);
}
