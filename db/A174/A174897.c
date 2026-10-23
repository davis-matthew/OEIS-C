/*
 * A174897  a(n) = characteristic function of numbers k such that A007955(m) =
 *          k has solution for some m, where A007955(m) = product of divisors
 *          of m.
 * https://oeis.org/A174897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9356, u16 n <= 9356, u32 n <= 9356, u64 n <= 9356,
 *            u128 n <= 9356, big any n
 * Helpers:   factor_u64, int64_to_seq_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS sequence A174897
 *
 * a(n) = 1 if there exists m such that A007955(m) = n,
 * where A007955(m) = product of divisors of m.
 *
 * For m = ∏ p_i^{a_i}, d(m) = ∏ (a_i+1) and
 * A007955(m) = m^{d(m)/2}.  Hence we look for integers e
 * such that n = m^e and d(m) = 2e (e integer), or
 * e = (odd)/2 so that n = m^e with m a square and
 * d(m) = odd (=2e).  The algorithm factors n once
 * and tests all possible exponents e (both integer
 * and half-integers) that divide the prime exponents
 * of n.  If a matching m is found, a(n) = 1; otherwise 0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/int64_to_seq_uint64.h"

seq_t a(idx_t n)
{
    if (n < (idx_t)1) return 0;                       /* offset is 1 */

    /* Since ISO C cannot cast very large seq_t to uint64_t safely,
       we assume all test n fit in 64 bits.  If not, this will
       still produce a correct answer for the values that fit.  */
    uint64_t un = (uint64_t)n;

    /* Factor n into primes and exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(un, primes, exps, &cnt);

    if (cnt == 0) { /* n == 1 */
        return 1;   /* 1 = (1)^{1} and has divisor count 1 */
    }

    /* Determine a rough upper bound for exponents e to test */
    uint64_t max_exp = 0;
    for (size_t i = 0; i < cnt; ++i)
        if (exps[i] > max_exp) max_exp = exps[i];
    /* e the integer exponent must not exceed max_exp,
       while odd e2 does not exceed max_exp as well.   */
    for (uint64_t e = 1; e <= max_exp; ++e) {
        /* --- integer exponent case: e integer, d(m)=2e --- */
        bool ok = true;
        for (size_t i = 0; i < cnt; ++i) {
            if ((exps[i] % e) != 0) { ok = false; break; }
        }
        if (ok) {
            seq_t divcnt = 1;
            for (size_t i = 0; i < cnt; ++i) {
                divcnt *= (int64_to_seq_uint64(exps[i] / e + 1));
            }
            if (divcnt == int64_to_seq_uint64(2 * e))
                return 1;
        }

        /* --- half-exponent case: e = (odd)/2, m must be square --- */
        if (e % 2 == 1) {   /* e is odd (so e2 = e) */
            bool ok2 = true;
            for (size_t i = 0; i < cnt; ++i) {
                if ((exps[i] % e) != 0) { ok2 = false; break; }
            }
            if (ok2) {
                seq_t divcnt2 = 1;
                for (size_t i = 0; i < cnt; ++i) {
                    divcnt2 *= (int64_to_seq_uint64(2 * (exps[i] / e) + 1));
                }
                if (divcnt2 == int64_to_seq_uint64(e))
                    return 1;
            }
        }
    }

    return 0;
}
