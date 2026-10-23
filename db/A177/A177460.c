/*
 * A177460  Smallest k such that A000217(n)+k and A000217(n)-k are both prime.
 * https://oeis.org/A177460
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k ≥ 0 such that T(n) – k and T(n) + k are prime,
 *            where T(n) = n*(n+1)/2 is the n‑th triangular number. Offset: 2
 *            (a(2) is the first term)
 * Range:     u8 n <= 22, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   is_prime_generic
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_generic.h"

seq_t a(idx_t n)
{
    /* a(2) is the first defined term; for n < 2 we return 0 as a
       harmless placeholder.  The caller is assumed to supply n ≥ 2. */
    if (n < 2)
        return 0;

    /* Convert n to the required seq_t type before arithmetic. */
    seq_t T = (seq_t)n * ((seq_t)n + 1) / 2;
    idx_t Tidx = (idx_t)T;  /* use idx_t for integer‑precision arithmetic */

    for (seq_t k = 0; k < T; ++k) {
        idx_t kidx = (idx_t)k;
        idx_t minus = Tidx - kidx;
        idx_t plus  = Tidx + kidx;
        if (minus >= 2 &&
            is_prime_generic(minus) &&
            is_prime_generic(plus))
            return k;
    }

    /* In practice a suitable k always exists for the tested range,
       so this point should never be reached. */
    return 0;
}
