/*
 * A086000  For p = prime(n), a(n) is the smallest N such that pN is a base-2
 *          pseudoprime (that is, 2^(pN-1) = 1 mod pN).
 * https://oeis.org/A086000
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 22, u32 n <= 778, u64 n <= 9858,
 *            u128 n <= 9858, big any n
 * Helpers:   build_spf, get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: For n>=2, a(n) is the smallest N such that p_n * N is a
 * base‑2 pseudoprime; p_n is the n‑th prime.  The algorithm computes N
 * as 1 + 2*ord(4,p_n)*k, incrementing k until the pseudoprime condition
 * holds. */
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/mod_pow.h"
#include "../utils/build_spf.h"

/* static smallest prime factor table (prepared once) */
static idx_t *SPF = NULL;
static const idx_t SPF_LIMIT = 2000000;

static void ensure_spf(void)
{
    if (SPF == NULL) {
        build_spf(SPF_LIMIT, &SPF);
    }
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;
    ensure_spf();

    /* prime p = p_n */
    uint32_t p_val = get_prime(n);      /* returns 32‑bit prime */
    seq_t p = (seq_t)p_val;

    /* compute ord_4(p) */
    uint64_t order = (uint64_t)p_val - 1;      /* start with p-1 */
    uint64_t temp  = order;
    while (temp > 1) {
        idx_t f = SPF[(idx_t)temp];
        if (f == 0) f = temp;                /* safety for 1 */
        /* reduce order by powers of f while 4^(order/f) ≡ 1 mod p */
        while (order % f == 0) {
            uint64_t candidate = order / f;
            if (mod_pow((seq_t)4, candidate, p) == (seq_t)1) {
                order = candidate;
            } else {
                break;
            }
        }
        /* remove factor f from temp */
        while (temp % f == 0) temp /= f;
    }

    /* search for the minimal N = 1 + 2*order*k */
    uint64_t k = 1;
    for (;;) {
        seq_t N = (seq_t)(1 + 2 * order * k);
        seq_t M = p * N;                 /* p * N  (composite odd) */
        uint64_t exp = (uint64_t)(M - 1); /* exponent fits in 64 bits */
        if (mod_pow((seq_t)2, exp, M) == (seq_t)1) {
            return N;
        }
        ++k;
    }
}
