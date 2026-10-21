/*
 * A079869  a(1)=1 and for n>1: round(n^(1/Omega(n)))^Omega(n), where Omega(n)
 *          is the total number of prime factors of n (A001222).
 * https://oeis.org/A079869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(n^(1/Omega(n)))^Omega(n), a(1)=1
 * Range:     u8 n <= 39, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   bigomega, pow_with_limit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Omega(n) = total number of prime factors of n with multiplicity */

#include <stdint.h>
#include "../utils/bigomega.h"
#include "../utils/pow_with_limit.h"

static seq_t pow_exact(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (exp)
            base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;

    /* Omega(n) */
    seq_t omega_seq = bigomega(n);
    idx_t omega = (idx_t)omega_seq;

    /* For omega == 1, the result is n itself */
    if (omega == 1)
        return (seq_t)n;

    seq_t n_val = (seq_t)n;

    /* Find integer R = floor(n^(1/omega)) */
    idx_t lo = 1, hi = (idx_t)n, R = 1;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (pow_with_limit((seq_t)mid, omega, n_val) <= n_val) {
            R = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }

    /* Decide whether to round up or keep R.
       Compare n with ((R + 0.5)^omega) via integer arithmetic:
       n * 2^omega <= (2*R + 1)^omega  => round to R
       else round to R+1.                 */

    /* Compute lhs = n * 2^omega */
    seq_t two_pow = pow_exact((seq_t)2, omega);
    seq_t lhs = n_val * two_pow;

    /* Compute rhs = (2*R + 1)^omega, but stop if it exceeds lhs */
    seq_t rhs = pow_with_limit((seq_t)(2 * R + 1), omega, lhs);

    seq_t root;
    if (rhs > lhs)
        root = (seq_t)R;
    else
        root = (seq_t)(R + 1);

    /* Return the rounded power: root^omega */
    return pow_exact(root, omega);
}
