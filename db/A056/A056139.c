/*
 * A056139  a(n) = n^2 - primefloor(n)*primeceiling(n).
 * https://oeis.org/A056139
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = n^2 - primefloor(n)*primeceiling(n)
 * Range:     i8 n <= 35, i16 n <= 1327, i32 n <= 8872, i64 n <= 8872,
 *            i128 n <= 8872, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(2) is the first term (offset 2) */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Undefined for n < 2; return 0 for safety */
    if (n < 2) return 0;

    /* Find largest prime <= n (primefloor) */
    idx_t p_floor = n;
    while (!is_prime(p_floor))
        --p_floor;          /* n ≥ 2 guarantees p_floor = 2 at worst */

    /* Find smallest prime >= n (primeceiling) */
    idx_t p_ceil = n;
    if (!is_prime(p_ceil)) {
        ++p_ceil;           /* start searching just above n */
        while (!is_prime(p_ceil))
            ++p_ceil;
    }

    /* Compute the value in seq_t, avoiding overflow of idx_t */
    seq_t nn = (seq_t)n;
    seq_t pf = (seq_t)p_floor;
    seq_t pc = (seq_t)p_ceil;

    return nn * nn - pf * pc;
}
