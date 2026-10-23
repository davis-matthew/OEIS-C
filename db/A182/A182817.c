/*
 * A182817  Number of elements k in Z/mZ such that k^m=k, for nonprime m =
 *          A018252(n).
 * https://oeis.org/A182817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 458, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A182817
   Formula: a(n) = number of k in Z/mZ with k^m = k,
   where m is the nth nonprime (A018252(n)).
   Implements the theoretical count:
     product over prime powers p^e dividing m of
     (1 + #units solving x^(m-1)=1 in (Z/p^eZ)^*),
   with the unit count described in the comments below. */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/is_prime.h"
#include "../utils/factor_u64.h"

static inline uint64_t gcd_uint64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Count solutions to k^m = k (mod m) for a given modulus m. */
static seq_t count_solutions_uint64(uint64_t m)
{
    if (m == 1)
        return (seq_t)1;                 /* only zero element */

    /* Prime factorise m */
    uint64_t primes[64], exps[64];
    size_t pfcnt = 0;
    factor_u64(m, primes, exps, &pfcnt);

    seq_t total = 1;
    uint64_t k = m - 1;                  /* exponent in x^(m-1)=1 */

    for (size_t i = 0; i < pfcnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t local = 0;                 /* count for this prime power */

        if (p == 2) {
            /* (Z/2^e)^* structure */
            if (e == 1 || e == 2) {
                /* units group is cyclic of order 1 or 2 -> one unit solution */
                local = (seq_t)2;        /* 1 (zero) + 1 unit */
            } else {
                /* e >= 3: group ≅ C2 × C_{2^{e-2}} */
                uint64_t pow2e   = 1ULL << (e - 2);        /* 2^(e-2) */
                uint64_t g       = gcd_uint64(k, pow2e);   /* in the cyclic part */
                uint64_t units   = (k & 1ULL) ? 1ULL : 2ULL; /* parity factor */
                units *= g;                                /* total unit solutions */
                local = (seq_t)(1 + units);                /* + zero */
            }
        } else {
            /* odd prime: cyclic group of order φ = p^(e-1)*(p-1) */
            uint64_t phi = 1;
            for (uint64_t j = 0; j < e - 1; ++j)
                phi *= p;
            phi *= (p - 1);
            uint64_t g = gcd_uint64(k, phi);
            local = (seq_t)(1 + g);  /* + zero */
        }

        total *= local;
    }

    return total;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Find the nth nonprime number (A018252). 1 is counted as nonprime. */
    uint64_t m = 0;
    idx_t count = 0;
    uint64_t cand = 1;
    for (;;) {
        if (!is_prime((idx_t)cand)) {
            ++count;
            if (count == n) {
                m = cand;
                break;
            }
        }
        ++cand;
    }

    return count_solutions_uint64(m);
}
