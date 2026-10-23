/*
 * A159817  Coefficients of L-series for elliptic curve "80b2": y^2 = x^3 -
 *          x^2 - x.
 * https://oeis.org/A159817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 50, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   factor_u64, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A159817: coefficients of the L‑series for the elliptic curve 80b2 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"
#include "../utils/factor_u64.h"

/* Compute a(n) = b(2*n + 1) where b is the multiplicative sequence
 * defined on odd integers by
 *   b(1)   = 1
 *   b(p^e) = b(p) * b(p^(e-1)) - p * b(p^(e-2))   for primes p ≠ 2,5
 *   b(2^e) = 0^e
 *   b(5^e) = (-1)^e
 *   b(m)   = ∏ b(p_i^e_i) for the prime factorisation m = ∏ p_i^e_i
 * The values b(p) (the trace of Frobenius) are computed by point counting
 * on the elliptic curve y² = x³ - x² - x over F_p.
 *
 * The function memoises the ap values to avoid repeated point counting.
 */

static seq_t *ap_cache = NULL;      /* ap value for prime p, indexed by p   */
static bool  *ap_flag  = NULL;      /* true if ap_cache[p] is valid        */
static size_t ap_size  = 0;         /* current size of the arrays          */

/* Ensure that ap_cache/ap_flag are large enough to store entries up to index 'limit'. */
static void _ensure_ap_size(size_t limit)
{
    if (limit <= ap_size) return;
    size_t new_size = limit;
    seq_t *new_cache = (seq_t *)realloc(ap_cache, new_size * sizeof(seq_t));
    bool  *new_flag  = (bool  *)realloc(ap_flag , new_size * sizeof(bool));
    if (!new_cache || !new_flag) exit(EXIT_FAILURE);
    for (size_t i = ap_size; i < new_size; ++i) new_flag[i] = false;
    ap_cache = new_cache;
    ap_flag  = new_flag;
    ap_size  = new_size;
}

/* Point counting for a prime p: compute a_p = p + 1 - #points on the
 * curve y² = x³ - x² - x modulo p.  The count is obtained via Legendre
 * symbols: a_p = -∑_{x∈F_p} ( (x³ - x² - x | p) ).  This uses only
 * modular arithmetic, no heavy algebraic geometry. */
static seq_t _compute_ap(uint64_t p)
{
    seq_t P = (seq_t)p;
    seq_t sum = 0;  /* Legendre symbol sum; will stay within |p|         */
    for (uint64_t x = 0; x < p; ++x)
    {
        seq_t t  = (seq_t)(x % p);
        seq_t t2 = (t * t) % P;
        seq_t t3 = (t2 * t) % P;
        seq_t rhs = t3 - t2 - t;
        rhs %= P;
        if (rhs < 0) rhs += P;
        int leg;
        if (rhs == 0)          leg = 0;
        else
        {
            seq_t pow = mod_pow(rhs, (p - 1) / 2, P);
            leg = (pow == (P - 1)) ? -1 : 1;
        }
        sum += leg;
    }
    return -(seq_t)sum;  /* a_p = -sum */
}

/* Retrieve cached a_p or compute if missing. */
static seq_t _get_ap(uint64_t p)
{
    _ensure_ap_size((size_t)p + 1);
    if (ap_flag[p]) return ap_cache[p];
    seq_t val = _compute_ap(p);
    ap_cache[p] = val;
    ap_flag[p]  = true;
    return val;
}

/* Compute b(m) for odd m via its multiplicative definition. */
static seq_t _compute_b(uint64_t m)
{
    if (m == 1) return 1;
    /* m is odd by construction; the following guard is defensive. */
    if ((m & 1ULL) == 0) return 0;

    seq_t result = 1;
    /* factorize m */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t cnt = 0;
    factor_u64(m, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t val;

        if (p == 2)            /* not needed for odd m */
            val = 0;
        else if (p == 5)
            val = (e & 1ULL) ? -1 : 1;
        else
        {
            seq_t ap = _get_ap(p);
            if (e == 1)
                val = ap;
            else
            {
                seq_t prevprev = 1;   /* b(p^0) */
                seq_t prev     = ap;  /* b(p^1) */
                seq_t curr = 0;
                for (uint64_t k = 2; k <= e; ++k)
                {
                    curr = ap * prev - (seq_t)p * prevprev;
                    prevprev = prev;
                    prev     = curr;
                }
                val = curr;
            }
        }

        result *= val;
        if (result == 0) break;  /* a zero factor kills the whole product */
    }

    return result;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    uint64_t m = 2 * (uint64_t)n + 1;   /* mapping n -> odd integer */
    return _compute_b(m);
}
