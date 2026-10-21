/*
 * A090816  a(n) = (3*n+1)!/((2*n)! * n!).
 * https://oeis.org/A090816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n+1)!/((2*n)! * n!)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   gcd_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses a multiplicative binomial computation:
   a(n) = (2*n+1) * binom(3*n+1, n) */
#include <stdint.h>
#include "../utils/gcd_seq.h"

#ifdef __SIZEOF_INT128__
#define HAVE_INT128 1
#else
#define HAVE_INT128 0
#endif

seq_t a(idx_t n) {
    /* compute binom(3*n+1, n) */
    seq_t result = 1;
    /* Use smaller k to minimise work (here k = n, but 3*n+1-k = 2*n+1 which is larger).
       Since n <= 3*n+1 - n, we keep k = n. */
    for (idx_t i = 1; i <= n; ++i) {
        /* numerator = (3*n+1) - i + 1 = 3*n - i + 2 */
        seq_t num = (seq_t)(3 * n - i + 2);
        seq_t den = (seq_t)i;

        /* Reduce gcd with current result to keep values small */
        seq_t g1 = gcd_seq(result, den);
        result /= g1;
        den /= g1;

        seq_t g2 = gcd_seq(num, den);
        num /= g2;
        den /= g2;   /* should become 1 */

        /* Multiply result by num safely */
        if (sizeof(seq_t) <= 8 && HAVE_INT128) {
            unsigned __int128 tmp = (unsigned __int128)result * (unsigned __int128)num;
            result = (seq_t)tmp;
        } else {
            result *= num;
        }
    }

    /* multiply by (2*n+1) */
    seq_t mult = (seq_t)(2 * n + 1);
    if (sizeof(seq_t) <= 8 && HAVE_INT128) {
        unsigned __int128 tmp = (unsigned __int128)result * (unsigned __int128)mult;
        result = (seq_t)tmp;
    } else {
        result *= mult;
    }

    return result;
}
