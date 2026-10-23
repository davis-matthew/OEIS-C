/*
 * A165092  Length of preperiodic part of trajectory of n under iteration of
 *          the base-8 Kaprekar map in A165090.
 * https://oeis.org/A165092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = length of preperiod of the trajectory of n under the
 *            base‑8 Kaprekar map (A165090)
 * Range:     u8 n <= 65, u16 n <= 4096, u32 n <= 4096, u64 n <= 4096,
 *            u128 n <= 4096, big any n
 * Helpers:   kaprec_oct
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses Floyd’s cycle‑finding algorithm to compute the preperiod length. */

#include <stdint.h>
#include "../utils/kaprec_oct.h"

seq_t a(idx_t n)
{
    seq_t x0 = (seq_t)n;

    // Phase 1: Find a repetition
    seq_t tortoise = kaprec_oct(x0);
    seq_t hare = kaprec_oct(kaprec_oct(x0));
    while (tortoise != hare) {
        tortoise = kaprec_oct(tortoise);
        hare = kaprec_oct(kaprec_oct(hare));
    }

    // Phase 2: Find the start of the cycle (mu = preperiod length)
    tortoise = x0;
    seq_t mu = 0;
    while (tortoise != hare) {
        tortoise = kaprec_oct(tortoise);
        hare = kaprec_oct(hare);
        mu++;
    }

    return mu;
}
