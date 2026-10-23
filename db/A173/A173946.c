/*
 * A173946  Numbers n such that omega(lambda(n)) = lambda(omega(n)).
 * https://oeis.org/A173946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth integer satisfying ω(λ(n)) = λ(ω(n)) (ω = number of
 *            distinct primes, λ = Carmichael function). The computation scans
 *            natural numbers and keeps those that satisfy the condition
 * Range:     u8 n <= 59, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_distinct_primes, factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Use utility helpers from the build */
#include "../utils/count_distinct_primes.h"
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"

/* Compute Carmichael λ(n) for n>=1.
   λ(1) = 1.  For an odd prime power p^e, λ = φ(p^e) = p^(e‑1)(p‑1).
   For 2^e (e≥3) λ = 2^(e‑2); for 2 and 4 it is 1 and 2 respectively.
   The result is returned as a seq_t value. */
static seq_t lambda_of_uint64(uint64_t n)
{
    if (n <= 1) {
        return 1;
    }

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        size_t e = (size_t)exps[i];
        seq_t term;

        if (p == 2 && e >= 3) {
            /* λ(2^e) = 2^(e-2) for e≥3 */
            term = 1ULL << (e - 2);
        } else {
            /* λ(p^e) = φ(p^e) = p^(e-1)*(p-1) */
            seq_t pow = 1;
            for (size_t k = 0; k < e - 1; ++k) {
                pow *= (seq_t)p;
            }
            term = pow * ((seq_t)p - 1);
        }

        /* lcm(result, term) = result / gcd(result, term) * term */
        seq_t g = gcd(result, term);
        result = result / g * term;
    }
    return result;
}

/* Test whether integer n satisfies ω(λ(n)) = λ(ω(n)). */
static bool satisfies(uint64_t n)
{
    if (n == 1) {
        /* By definition 1 is a member of the sequence. */
        return true;
    }

    /* ω(n) */
    int omega_n = count_distinct_primes(n);

    /* λ(n) */
    seq_t lam_n = lambda_of_uint64(n);

    /* ω(λ(n)) as an integer */
    int omega_lam_n = count_distinct_primes((uint64_t)lam_n);

    /* λ(ω(n)) as a seq_t value */
    seq_t lam_omega_n = lambda_of_uint64((uint64_t)omega_n);

    /* Condition: ω(λ(n)) == λ(ω(n)) */
    return ((seq_t)omega_lam_n) == lam_omega_n;
}

/* Returns the n‑th term of OIES A173946 (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }

    idx_t found = 0;
    uint64_t candidate = 1;

    while (found < n) {
        if (satisfies(candidate)) {
            ++found;
            if (found == n) {
                return (seq_t)candidate;
            }
        }
        ++candidate;
    }
    /* This point is never reached. */
    return 0;
}
