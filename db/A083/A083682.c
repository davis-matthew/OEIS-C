/*
 * A083682  Sum of aliquot divisors of semiprimes.
 * https://oeis.org/A083682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum of proper divisors of the n‑th semiprime * * For the
 *            n‑th semiprime p*q (p ≤ q) * if p == q → a(n) = 1 + p (since the
 *            proper divisors are 1 and p) * else → a(n) = 1 + p + q (proper
 *            divisors are 1, p, q) * * The routine builds a table of primes
 *            once and factorises each * candidate integ...
 * Range:     u8 n <= 157, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/build_primes.h"   /* build_primes(idx_t limit, idx_t **primes, size_t *cnt) */

static idx_t *prime_tbl = NULL;   /* array of primes (uint64_t) */
static size_t prime_cnt  = 0;     /* number of primes */

/* Initialise the prime table once (bound is conservative for n≈10 000). */
static void init_primes(void)
{
    if (prime_tbl != NULL)
        return;

    /* The 10 000th semiprime is < 200 000; using 1 000 000 gives a buffer. */
    const idx_t UPPER = 1000000;
    build_primes(UPPER, &prime_tbl, &prime_cnt);
}

/* ------------------------------------------------------------
 *  Determine whether n is a semiprime.
 *
 *  If true, *p1 and *p2 receive its two prime factors (equal for a square).
 *  The function returns false otherwise.
 * ------------------------------------------------------------ */
static bool semiprime_factors(uint64_t n, uint64_t *p1, uint64_t *p2)
{
    if (n < 4)            /* smallest semiprime is 4 = 2² */
        return false;

    uint64_t rem = n;
    uint64_t f1 = 0, f2 = 0;
    uint8_t  e1 = 0, e2 = 0;          /* exponents of the primes */

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = (uint64_t)prime_tbl[i];
        if (p * p > rem)           /* remaining factor is prime */
            break;

        if (rem % p == 0) {
            uint8_t exp = 0;
            while (rem % p == 0) {
                rem /= p;
                ++exp;
                if (exp > 2)            /* exponent > 2 → not a semiprime */
                    return false;
            }

            if (f1 == 0) {
                f1 = p;
                e1 = exp;
            } else if (f2 == 0) {
                f2 = p;
                e2 = exp;
            } else                     /* more than two distinct primes */
                return false;
        }
    }

    /* The remaining part (if any) is a prime factor with exponent 1. */
    if (rem > 1) {
        if (f1 == 0) {
            f1 = rem;
            e1 = 1;
        } else if (f2 == 0) {
            f2 = rem;
            e2 = 1;
        } else
            return false;
    }

    /* Total exponent sum must be exactly 2 for a semiprime. */
    if (e1 + e2 != 2)
        return false;

    /* Arrange the return values.  For p², set p2 = p1. */
    *p1 = f1;
    *p2 = (e2 == 0) ? f1 : f2;
    return true;
}

/* ------------------------------------------------------------
 *  Required sequence function.
 * ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)            /* lower indices are undefined; return 0 safely */
        return (seq_t)0;

    init_primes();

    idx_t  count = 0;      /* how many semiprimes have been found */
    uint64_t num = 4;      /* first semiprime */

    while (true) {
        uint64_t p1, p2;
        if (semiprime_factors(num, &p1, &p2)) {
            ++count;
            if (count == n) {
                seq_t res;
                /* For a square p² the output is 1 + p.  For a product p*q it is
                 * 1 + p + q.  The exponent test in semiprime_factors ensures
                 * that p == q only for a square.                    */
                if (p1 == p2)      /* square */
                    res = (seq_t)(1 + p1);
                else
                    res = (seq_t)(1 + p1 + p2);
                return res;
            }
        }
        ++num;
    }
}
