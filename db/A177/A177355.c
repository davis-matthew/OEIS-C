/*
 * A177355  The number of positive integers m for which the exponents of
 *          prime(n) and prime(n+1) in the prime power factorization of m! are
 *          both powers of 3.
 * https://oeis.org/A177355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 17, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   gather_intervals, get_prime, legendre_vp, lower_bound_vp,
 *            upper_bound_vp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = count of m > 0 such that the exponents of the
 * prime(n) and prime(n+1) in m! are both powers of 3.
 * The algorithm:
 *   1. Let p = prime(n), q = prime(n+1).
 *   2. All admissible m lie in [q, U), with
 *      U = floor(2*(q^2)*(log(2)/(2*log(q)-1)+1)).
 *   3. For each power of 3 (1,3,9,…) not exceeding the
 *      maximum exponent of p or q in the range, determine the
 *      interval of m where the exponent of that prime equals
 *      that power (using binary search with Legendre's formula).
 *   4. Intersect the intervals for p and for q and sum their lengths.
 */

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/gather_intervals.h"
#include "../utils/legendre_vp.h"
#include "../utils/lower_bound_vp.h"
#include "../utils/upper_bound_vp.h"

static idx_t compute_U(uint32_t q)
{
    double dq    = (double)q;
    double bound = 2.0 * (-1.0 + dq * dq * (log(2.0) / (2.0 * log(dq) - 1.0) + 1.0));
    idx_t   U     = (idx_t)bound;          /* floor */
    if (U < q) U = q + 1;                 /* safety, avoid empty interval */
    return U;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* Primes p = prime(n), q = prime(n+1) */
    uint32_t p = get_prime(n);
    uint32_t q = get_prime(n + 1);

    /* Upper bound of interval [q, U) */
    idx_t U = compute_U(q);
    if (U <= q) return (seq_t)0;          /* no admissible m */

    idx_t max_m = U - 1;                  /* inclusive upper bound */
    idx_t lower_limit = q;                /* m must be at least q */

    /* Gather intervals for p and q */
    idx_t Lp[20], Rp[20], Lq[20], Rq[20];
    idx_t cnt_p, cnt_q;
    gather_intervals(p, max_m, &cnt_p, &Lp, &Rp);
    gather_intervals(q, max_m, &cnt_q, &Lq, &Rq);

    seq_t result = 0;

    for (idx_t i = 0; i < cnt_p; ++i) {
        idx_t lp = Lp[i];
        idx_t rp = Rp[i];
        if (lp < lower_limit) lp = lower_limit;
        if (rp > max_m) rp = max_m;
        if (lp > rp) continue;

        for (idx_t j = 0; j < cnt_q; ++j) {
            idx_t lq = Lq[j];
            idx_t rq = Rq[j];
            if (lq < lower_limit) lq = lower_limit;
            if (rq > max_m) rq = max_m;
            if (lq > rq) continue;

            idx_t L = (lp > lq) ? lp : lq;
            idx_t R = (rp < rq) ? rp : rq;
            if (L <= R) {
                result += (seq_t)(R - L + 1);
            }
        }
    }

    return result;
}
