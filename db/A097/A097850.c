/*
 * A097850  a(n) = floor(2*sqrt(n)*phi(n)) - n.
 * https://oeis.org/A097850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(2*sqrt(n)*phi(n)) - n
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 1022, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/calc_phi.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. For the unlikely case
       n == 0 we simply return 0, but this is never used. */
    if (n == 0)
        return (seq_t)0;

    seq_t phi_n = calc_phi((seq_t)n);        /* φ(n) */
    seq_t phi_sq = phi_n * phi_n;            /* φ(n)^2 */
    seq_t m = (seq_t)4 * phi_sq * (seq_t)n;  /* 4·n·φ(n)^2 */
    seq_t s = floor_sqrt(m);                 /* floor( sqrt(m) ) */
    return s - (seq_t)n;                     /* a(n) */
}
