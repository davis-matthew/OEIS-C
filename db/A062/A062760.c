/*
 * A062760  a(n) is n divided by the largest power of the squarefree kernel of
 *          n (A007947) which divides it.
 * https://oeis.org/A062760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n / (product of p^min_exp(p) for all prime p | n) * where
 *            min_exp(p) = min{v_p(n) : p | n}. The exponent min_exp is the *
 *            smallest prime–exponent among the factorisation of n. The
 *            computation * is done by explicit trial‑division on n, storing
 *            the distinct primes * and their exponents; the...
 * Range:     u8 n <= 357, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Compute a(n) for any unsigned 64‑bit index n.  The function is
 * independent of any external library; it performs only integer
 * arithmetic and a simple trial division factorisation. */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;         /* special case: a(1) = 1 */

    /* Work with a 64‑bit unsigned integer; idx_t is 64‑bit in all builds. */
    uint64_t nn = (uint64_t)n;

    /* Arrays to hold distinct primes and their exponents.  A 64‑bit number
     * can have at most 16 distinct prime factors, but we over‑allocate for
     * safety. */
    uint64_t primes[64];
    unsigned   exps[64];
    size_t     cnt = 0;

    /* Factorisation by trial division. */
    uint64_t tmp = nn;
    unsigned exp;

    /* Count factors of 2 first. */
    if (tmp % 2ULL == 0) {
        exp = 0;
        while (tmp % 2ULL == 0) {
            tmp /= 2ULL;
            exp++;
        }
        primes[cnt] = 2ULL;
        exps[cnt]   = exp;
        cnt++;
    }

    /* Count odd factors. */
    for (uint64_t p = 3ULL; p * p <= tmp; p += 2ULL) {
        if (tmp % p == 0) {
            exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                exp++;
            }
            primes[cnt] = p;
            exps[cnt]   = exp;
            cnt++;
        }
    }

    /* If remainder > 1 it is a prime factor with exponent 1. */
    if (tmp > 1) {
        primes[cnt] = tmp;
        exps[cnt]   = 1;
        cnt++;
    }

    /* Find the minimal exponent among all primes. */
    unsigned min_exp = exps[0];
    for (size_t i = 1; i < cnt; ++i) {
        if (exps[i] < min_exp) min_exp = exps[i];
    }

    /* Successively divide n by p^min_exp for each distinct prime p. */
    seq_t result = (seq_t)nn;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        for (unsigned k = 0; k < min_exp; ++k) {
            result /= p;          /* exact division */
        }
    }

    return result;
}
