/*
 * A095972  Number of quadratic nonresidues modulo n.
 * https://oeis.org/A095972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - A000224(n) Where A000224(n) is the number of
 *            quadratic residues modulo n
 * Range:     u8 n <= 287, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 91 terms through n = 9085 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute A000224(n) through prime factorisation.  */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* number of quadratic residues modulo 2^e (e >= 1)   */
static seq_t residues_power_of_two(uint32_t e)
{
    if (e == 1 || e == 2) return (seq_t)2;
    seq_t R_prevprev = (seq_t)2;          /* R(1) */
    seq_t R_prev = (seq_t)2;              /* R(2) */
    seq_t R_curr = 0;
    for (uint32_t i = 3; i <= e; ++i) {
        if (i == 3) {
            R_curr = (seq_t)3;
        } else {
            seq_t add = ((seq_t)1) << (i - 3);   /* 2^(i-3) */
            R_curr = R_prevprev + add;
        }
        R_prevprev = R_prev;
        R_prev = R_curr;
    }
    return R_curr;
}

/* number of quadratic residues modulo p^e for an odd prime p (e >= 1)  */
static seq_t residues_prime_power(uint64_t p, uint32_t e)
{
    if (e == 0) return (seq_t)1;
    /* R(0) = 1, R(1) = (p + 1)/2 */
    seq_t R_prevprev = (seq_t)1; /* R(0) */
    seq_t R_prev = (seq_t)(p + 1) / 2;   /* R(1) */
    if (e == 1) return R_prev;

    seq_t power = (seq_t)p; /* p^1 */
    seq_t R_curr = 0;
    for (uint32_t i = 2; i <= e; ++i) {
        power = power * (seq_t)p;           /* p^i */
        seq_t term = (power - power / (seq_t)p) / (seq_t)2;   /* (p^i - p^{i-1})/2 */
        R_curr = R_prevprev + term;         /* R(i) = R(i-2) + term */
        R_prevprev = R_prev;
        R_prev = R_curr;
    }
    return R_curr;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;               /* a(1) = 0 */

    uint64_t rem = (uint64_t)n;
    seq_t residues = 1;                        /* multiplicative identity */

    /* handle factor 2 */
    if ((rem & 1ULL) == 0) {
        uint32_t e = 0;
        while ((rem & 1ULL) == 0) {
            rem >>= 1;
            ++e;
        }
        residues *= residues_power_of_two(e);
    }

    /* factor odd primes by trial division */
    for (uint64_t p = 3; p * p <= rem; p += 2) {
        if (rem % p == 0) {
            uint32_t e = 0;
            while (rem % p == 0) {
                rem /= p;
                ++e;
            }
            residues *= residues_prime_power(p, e);
        }
    }

    /* remaining prime factor > sqrt(rem) */
    if (rem > 1) {
        /* rem is prime, exponent e = 1 */
        residues *= residues_prime_power(rem, 1);
    }

    return (seq_t)n - residues;
}
