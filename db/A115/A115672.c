/*
 * A115672  Coefficients of L-series for elliptic curve "35a3": y^2 + y = x^3
 *          + x^2 - x.
 * https://oeis.org/A115672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   not stated
 * Range:     i8 n <= 1092, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative recurrence: a(5^e)=(-1)^e, a(7^e)=1.
   For other primes p, a(p^e)=a(p)*a(p^(e-1)) - p*a(p^(e-2)); a(1)=1. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* fast modular exponentiation for uint64_t (used for Legendre symbol) */
static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp) {
        if (exp & 1) result = (uint64_t)((__int128)result * b % mod);
        b = (uint64_t)((__int128)b * b % mod);
        exp >>= 1;
    }
    return result;
}

/* Count points on y^2 + y = x^3 + x^2 - x over F_p and return a(p)=p+1-#points. */
static seq_t compute_a_prime(uint64_t p)
{
    seq_t points = 1;                      /* point at infinity */
    seq_t sp = (seq_t)p;
    if (p == 2) {                          /* characteristic 2: special handling */
        for (uint64_t x = 0; x < p; ++x) {
            seq_t xx = ((seq_t)x * (seq_t)x) % sp;
            seq_t t1 = (xx * (seq_t)x) % sp;          /* x^3 */
            seq_t rhs = (t1 + xx + (sp - (seq_t)x)) % sp; /* x^3 + x^2 - x */
            seq_t sol = (rhs == 0) ? 2 : 0;
            points += sol;
        }
    } else {
        for (uint64_t x = 0; x < p; ++x) {
            seq_t xx = ((seq_t)x * (seq_t)x) % sp;
            seq_t t1 = (xx * (seq_t)x) % sp;          /* x^3 */
            seq_t rhs = (t1 + xx + (sp - (seq_t)x)) % sp; /* x^3 + x^2 - x */
            seq_t four = (4 * rhs) % sp;
            seq_t D = (four + 1) % sp;                /* discriminant 1 + 4*rhs */
            seq_t sol;
            if (D == 0) {
                sol = 1;
            } else {
                seq_t leg = mod_pow_uint64((uint64_t)D, (p - 1) / 2, p);
                sol = (leg == 1) ? 2 : 0;
            }
            points += sol;
        }
    }
    return (seq_t)p + 1 - points;
}

/* Memoised a(p) for primes */
static seq_t get_a_prime(uint64_t p)
{
    static uint64_t stored_p[128];
    static seq_t stored_ap[128];
    static size_t stored_cnt = 0;

    for (size_t i = 0; i < stored_cnt; ++i)
        if (stored_p[i] == p)
            return stored_ap[i];

    seq_t ap = compute_a_prime(p);
    if (stored_cnt < 128) {
        stored_p[stored_cnt]   = p;
        stored_ap[stored_cnt]  = ap;
        ++stored_cnt;
    }
    return ap;
}

/* Return the n-th term of OEIS A115672 (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n <= 1) return 1;           /* a(1)=1, a(0) undefined but return 1 */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t val;

        if (p == 5) {                 /* a(5^e)=(-1)^e */
            val = (e & 1) ? (seq_t)-1 : (seq_t)1;
        } else if (p == 7) {           /* a(7^e)=1 for all e≥1 */
            val = (seq_t)1;
        } else {
            seq_t ap = get_a_prime(p);
            if (e == 1) {
                val = ap;
            } else {
                seq_t prevprev = 1;   /* a(p^0) */
                seq_t prev = ap;      /* a(p^1) */
                seq_t curr;
                for (uint64_t exp = 2; exp <= e; ++exp) {
                    curr = ap * prev - (seq_t)p * prevprev;
                    prevprev = prev;
                    prev = curr;
                }
                val = curr;
            }
        }
        result = result * val;
    }
    return result;
}
