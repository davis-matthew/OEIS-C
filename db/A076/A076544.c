/*
 * A076544  a(n) = mu(n) + sqf(n) where mu(n) is Moebius function, sqf(n) = 1
 *          if n is squarefree and sqf(n) = -1 otherwise.
 * https://oeis.org/A076544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = mu(n) + sqf(n) where mu is the Möbius function and sqf(n)
 *            = 1 if n is squarefree, -1 otherwise. The same value can be
 *            obtained from a(n) = 2*mu(n)^2 + mu(n) - 1, which is computed
 *            here using the provided mu_of utility
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   mu_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mu_of.h"

seq_t a(idx_t n) {
    int mu = mu_of(n);          /* Möbius value: -1, 0, or 1 */
    if (mu == 0)                /* not squarefree */
        return (seq_t)-1;
    /* mu = ±1 and the number is squarefree */
    return (seq_t)(mu + 1);     /* 2 when mu=1, 0 when mu=-1 */
}
