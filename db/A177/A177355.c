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
 * Validated: partial, pass, 17 terms through n = 17 (2026-11-01)
 * Helpers:   none
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
#include <stdlib.h>
#include <stddef.h>

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

static idx_t legendre_vp(idx_t m, uint32_t p)
{
    idx_t exp = 0;
    while (m) {
        m /= p;
        exp += m;
    }
    return exp;
}

static idx_t lower_bound_vp(idx_t low, idx_t high, uint32_t p, idx_t target)
{
    idx_t lo = low, hi = high, res = high + 1;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (legendre_vp(mid, p) >= target) {
            res = mid;
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }
    return res;
}

static idx_t upper_bound_vp(idx_t low, idx_t high, uint32_t p, idx_t target)
{
    idx_t lo = low, hi = high, res = high + 1;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (legendre_vp(mid, p) > target) {
            res = mid;
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }
    return res;
}

static void gather_intervals(uint32_t prime, idx_t max_m,
                             idx_t *count, idx_t (*L)[20], idx_t (*R)[20])
{
    idx_t max_exp = legendre_vp(max_m, prime);
    idx_t power = 1;
    *count = 0;
    while (power <= max_exp) {
        idx_t l = lower_bound_vp(1, max_m, prime, power);
        idx_t r = upper_bound_vp(1, max_m, prime, power) - 1;

        /* clamp to admissible range [q, max_m] will be done later */
        if (l < 1) l = 1;
        if (r > max_m) r = max_m;

        if (l <= r) {
            (*L)[*count] = l;
            (*R)[*count] = r;
            (*count)++;
        }

        if (power > max_exp / 3) break;    /* avoid overflow of power*3 */
        power *= 3;
    }
}

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
