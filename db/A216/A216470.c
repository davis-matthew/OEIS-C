/*
 * A216470  a(n) = smallest m such that 2n-1 | (10^m+1)/11, or 0 if no such m
 *          exists.
 * https://oeis.org/A216470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   smallest m with 2n-1 | (10^m+1)/11, 0 if none. Computation:
 *            solve 10^m ≡ -1 (mod 11*(2n-1)) (1) If (2n-1) divisible by 5 or
 *            gcd(10, 11*(2n-1))≠1, return 0. Otherwise, let M = 11*(2n-1).
 *            Find multiplicative order ord of 10 modulo M. If ord is even and
 *            10^(ord/2) ≡ -1 (mod M), the answer is ord/2;...
 * Range:     u8 n <= 23, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset starts at 1 */

    /* 2n-1 */
    idx_t odd = 2*n - 1;
    if (odd % 5 == 0) return 0;      /* gcd(10,M)≠1, no solution */

    seq_t M = (seq_t)odd * (seq_t)11;        /* M = 11*(2n-1) */

    /* φ(M) */
    seq_t M_tmp = M;
    seq_t phi = M;
    for (idx_t p = 2; (seq_t)p * p <= M_tmp; ++p) {
        if (M_tmp % p == 0) {
            phi = (phi / (seq_t)p) * ((seq_t)p - 1);
            while (M_tmp % p == 0) M_tmp /= p;
        }
    }
    if (M_tmp > 1) {
        phi = (phi / (seq_t)M_tmp) * ((seq_t)M_tmp - 1);
    }

    /* order reduction of 10 modulo M based on φ(M) */
    seq_t ord = phi;
    seq_t phi_tmp = phi;
    for (idx_t p = 2; (seq_t)p * p <= phi_tmp; ++p) {
        if (phi_tmp % p == 0) {
            /* p is a prime factor of ord */
            while (ord % (seq_t)p == 0) {
                seq_t cand = ord / (seq_t)p;
                if (mod_pow((seq_t)10, (uint64_t)cand, M) == 1) {
                    ord = cand;
                } else {
                    break;
                }
            }
            while (phi_tmp % p == 0) phi_tmp /= p;
        }
    }
    if (phi_tmp > 1) {            /* remaining prime factor */
        seq_t p = (seq_t)phi_tmp;
        while (ord % p == 0) {
            seq_t cand = ord / p;
            if (mod_pow((seq_t)10, (uint64_t)cand, M) == 1) {
                ord = cand;
            } else {
                break;
            }
        }
    }

    /* check for -1 */
    if (ord % 2 == 0) {
        seq_t half = ord / 2;
        if (mod_pow((seq_t)10, (uint64_t)half, M) == M - 1) {
            return half;
        }
    }
    return 0;
}
