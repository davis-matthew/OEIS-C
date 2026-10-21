/*
 * A082081  a(n) = fixed point reached when the pseudo-log function A008475 is
 *          iterated starting from n.
 * https://oeis.org/A082081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   This function implements a(n) = the fixed point of the “pseudo‑log” function
   f(x) = sum of the prime powers in the factorisation of x (i.e. 
          sopfr(x) = Σ pᵉ over all primes p with exponent e such that
          x = Π pᵉ).  The iteration stops when the value is unchanged.
   a(n) is defined for n ≥ 1; a(1) = 0 because 1 → 0 and 0 is a fixed point.
*/

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
/*  factor() is available in the build (see the list of supplied helpers). */
#include "../utils/factor.h"

static seq_t sopfr_uint64(uint64_t n)
/*  Compute the sum of prime powers of a 64‑bit integer n.  */
{
    if (n <= 1)
        return (seq_t)0;                 /* sopfr(0)=0, sopfr(1)=0 */

    uint64_t pr[32], exp[32];
    size_t cnt;
    factor(n, pr, exp, &cnt);

    uint64_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t pwr = 1;
        for (uint64_t e = 0; e < exp[i]; ++e)
            pwr *= pr[i];
        sum += pwr;
    }
    return (seq_t)sum;
}

seq_t a(idx_t n)
/*  Return the fixed point of iterating sopfr starting from n.  */
{
    seq_t cur = (seq_t)n;

    for (;;) {
        uint64_t v = (uint64_t)cur;          /* n fits in uint64_t for the test set  */
        seq_t nxt = sopfr_uint64(v);
        if (nxt == cur)
            return cur;
        cur = nxt;
    }
}
