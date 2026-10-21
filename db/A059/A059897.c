/*
 * A059897  Symmetric square array read by antidiagonals: A(n,k) is the
 *          product of all factors that occur in one, but not both, of the
 *          Fermi-Dirac factorizations of n and k.
 * https://oeis.org/A059897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, tabl, nice, look
 * Formula:   not stated
 * Range:     u8 n <= 508, u16 n <= 8906, u32 n <= 8906, u64 n <= 8906,
 *            u128 n <= 8906, big any n
 * Helpers:   build_primes_upto, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A059897 – Symmetric square array read by antidiagonals:
    a(n) = A(i,j) where i and j are the antidiagonal coordinates
    of index n, and A(i,j) = ∏ p ^ (exp_i(p) XOR exp_j(p)).
    i.e. for each prime p, raise p to the xor of its exponents
    in i and j. The array is symmetric and every element is
    a divisor of i*j. This implementation computes a(n) for any
    n ≥ 1 (O(√n) factorisation, O(√n) time per call).              */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/seq_mul.h"

/* -------------  global cache of primes ------------- */
static uint64_t *primes = NULL;      /* prime list, sorted */
static size_t primes_cnt = 0;        /* number of primes */
static idx_t   primes_limit = 0;     /* largest value for which primes are ready */

static void
ensure_primes(idx_t limit)
{
    if (limit <= primes_limit) return;
    size_t  cnt;
    uint64_t *list;
    build_primes_upto(limit, &list, &cnt);   /* build primes ≤ limit       */
    if (primes) free(primes);
    primes = list;
    primes_cnt = cnt;
    primes_limit = limit;
}

/* -------------  integer sqrt (floor) ------------- */
static idx_t
int_sqrt(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo + 1) >> 1);
        if ((unsigned __int128)mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* -------------  prime factorisation ------------- */
static void
factor_into(idx_t n, idx_t *out_pr, idx_t *out_exp, size_t *cnt)
{
    size_t c = 0;
    idx_t  rem = n;
    for (size_t pi = 0; pi < primes_cnt &&
                       (uint64_t)primes[pi] * primes[pi] <= rem; ++pi) {
        idx_t p = primes[pi];
        idx_t e = 0;
        while (rem % p == 0) {
            rem /= p;
            ++e;
        }
        if (e) {
            out_pr[c]   = p;
            out_exp[c]  = e;
            ++c;
        }
    }
    if (rem > 1) {          /* remaining prime > sqrt(n)            */
        out_pr[c]   = rem;
        out_exp[c]  = 1;
        ++c;
    }
    *cnt = c;
}

/* -------------  small integer power for seq_t ------------- */
static seq_t
pow_seq_small(seq_t base, idx_t exp)
{
    seq_t res = 1;
    while (exp) {
        if (exp & 1)          res = seq_mul(res, base);
        exp >>= 1;
        if (exp) base = seq_mul(base, base);
    }
    return res;
}

/* -------------  XOR‑product of two integers ------------- */
static seq_t
xor_product(idx_t a, idx_t b)
{
    idx_t pa[64], ea[64], pb[64], eb[64];
    size_t ca = 0, cb = 0;
    factor_into(a, pa, ea, &ca);
    factor_into(b, pb, eb, &cb);

    seq_t res = 1;
    size_t ia = 0, ib = 0;

    while (ia < ca && ib < cb) {
        if (pa[ia] == pb[ib]) {
            idx_t e = ea[ia] ^ eb[ib];
            if (e)  res = seq_mul(res, pow_seq_small(pa[ia], e));
            ++ia; ++ib;
        } else if (pa[ia] < pb[ib]) {
            if (ea[ia]) res = seq_mul(res, pow_seq_small(pa[ia], ea[ia]));
            ++ia;
        } else { /* pb[ib] < pa[ia] */
            if (eb[ib]) res = seq_mul(res, pow_seq_small(pb[ib], eb[ib]));
            ++ib;
        }
    }
    while (ia < ca) {
        if (ea[ia]) res = seq_mul(res, pow_seq_small(pa[ia], ea[ia]));
        ++ia;
    }
    while (ib < cb) {
        if (eb[ib]) res = seq_mul(res, pow_seq_small(pb[ib], eb[ib]));
        ++ib;
    }
    return res;
}

/* -------------  sequence a(n) ------------- */
seq_t
a(idx_t n)
{
    if (n == 0) return 0;        /* defined only for n≥1 */

    /* compute antidiagonal coordinates (i,j) for index n */
    idx_t g = 8 * n - 7;                     /* 8n-7 is always ≥1 for n≥1 */
    idx_t s = int_sqrt(g);                   /* floor(sqrt(8n-7))          */
    idx_t k = (s - 1) / 2 + 1;               /* diagonal number (sum of indices) */
    idx_t prev = k * (k - 1) / 2;            /* T_{k-1} (prev triangular)   */
    idx_t pos  = n - prev;                   /* position on this diagonal, 1‑based */
    idx_t i = pos;
    idx_t j = k + 1 - pos;

    /* ensure prime table covers both coordinates */
    idx_t maxv = (i > j) ? i : j;
    ensure_primes(maxv);

    return xor_product(i, j);
}
