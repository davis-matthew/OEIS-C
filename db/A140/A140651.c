/*
 * A140651  A140579^(-1) * A000290, the squares starting (1, 4, 9, ...).
 * https://oeis.org/A140651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * LCM({d : d|n, d < n}) Equivalent to: if n == 1 -> 1
 *            if n is a prime power p^e -> p^(2e-1) otherwise -> n^2
 * Range:     u8 n <= 17, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/factor_u64.h"

/* External linkage as required */
seq_t a(idx_t n)
{
    /* Handle offset: a(1) = 1 */
    if (n == 1)
        return 1;

    /* Factor n */
    static const size_t MAX_FACTORS = 16;   /* enough for uint64_t */
    uint64_t pr[MAX_FACTORS];
    uint64_t ex[MAX_FACTORS];
    size_t   cnt = 0;

    factor_u64((uint64_t)n, pr, ex, &cnt);

    /* If n has only one distinct prime factor (including primes themselves) */
    if (cnt == 1) {
        idx_t e = (idx_t)ex[0];                /* exponent of the prime */
        idx_t exp = 2 * e - 1;                 /* the power required */
        return big_pow((seq_t)pr[0], (seq_t)exp);
    }

    /* For composite numbers with at least two distinct primes */
    seq_t ns = (seq_t)n;
    return ns * ns;
}
