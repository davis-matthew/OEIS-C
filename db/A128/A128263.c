/*
 * A128263  Coefficients of L-series for elliptic curve "17a4": y^2 + x*y + y
 *          = x^3 - x^2 - x or y^2 + x*y - y = x^3 - x^2.
 * https://oeis.org/A128263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) is multiplicative. a(1) = 1. For p = 17: a(17^e) = 1 for
 *            all e ≥ 1. For any other prime p: a(p) = p + 1 – N_p, where N_p
 *            is the number of points on the elliptic curve y^2 + x*y + y =
 *            x^3 – x^2 – x over GF(p). For e ≥ 2: a(p^e) = a(p)·a(p^(e-1)) –
 *            p·a(p^(e-2))
 * Range:     i8 n <= 2427, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Utility: modular exponentiation (base^exp mod mod), all values fit in seq_t. */
static seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod)
{
    seq_t result = 1;
    base %= mod;
    while (exp) {
        if (exp & 1ULL)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1ULL;
    }
    return result;
}

/* Count the number of points N_p on the curve over GF(p).  p is prime, p ≠ 17. */
static seq_t count_points_prime(uint64_t p)
{
    seq_t N = 1;                      /* the point at infinity */
    seq_t p_seq = (seq_t)p;
    seq_t four  = (seq_t)4;
    for (uint64_t x = 0; x < p; ++x) {
        seq_t x1 = (seq_t)x;
        /* rhs = x^3 – x^2 – x  (mod p) */
        seq_t x2 = (x1 * x1) % p_seq;
        seq_t x3 = (x2 * x1) % p_seq;
        seq_t rhs = (x3 + p_seq - (x2 + x1) % p_seq) % p_seq;

        /* Discriminant D = (x+1)^2 + 4*rhs  (mod p) */
        seq_t t = ((x1 + 1) * (x1 + 1)) % p_seq;
        seq_t D = (t + four * rhs) % p_seq;

        if (D == 0) {
            N += 1;                        /* one solution for y */
        } else {
            seq_t leg = mod_pow(D, (p - 1) / 2, p_seq);
            if (leg == 1) {
                N += 2;                    /* two quadratic solutions */
            }                                 /* else: no solutions */
        }
    }
    return N;
}

/* Compute a(p^e) for a prime p.  For p == 17, return 1 regardless of e. */
static seq_t apower(uint64_t p, uint64_t e)
{
    if (e == 0) return (seq_t)1;
    if (p == 17ULL) return (seq_t)1;

    seq_t ap = (seq_t)p + 1 - count_points_prime(p);   /* a(p) */
    if (e == 1) return ap;

    seq_t prev_prev = (seq_t)1;   /* a(p^0)   */
    seq_t prev     = ap;          /* a(p^1)   */
    seq_t curr;
    for (uint64_t k = 2; k <= e; ++k) {
        curr = ap * prev - (seq_t)p * prev_prev;
        prev_prev = prev;
        prev = curr;
    }
    return prev;
}

/* Main sequence function as required by the build. */
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;   /* a(1) = 1, offset 1 */
    uint64_t tn = (uint64_t)n;
    seq_t result = 1;

    /* Factorize tn by trial division.  tn ≤ 10000 in our usage. */
    for (uint64_t p = 2; p * p <= tn; ++p) {
        if (tn % p != 0) continue;
        uint64_t exp = 0;
        while (tn % p == 0) {
            tn /= p;
            ++exp;
        }
        result *= apower(p, exp);
    }

    /* Remaining prime factor > sqrt(original n). */
    if (tn > 1) {
        result *= apower(tn, 1);
    }

    return result;
}
