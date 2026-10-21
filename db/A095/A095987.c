/*
 * A095987  a(n) = gcd(n!!, (n-1)!!) where n!! = A006882.
 * https://oeis.org/A095987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n!!, (n-1)!!)
 * Range:     u8 n <= 13, u16 n <= 21, u32 n <= 33, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Utility functions for arbitrary‑precision integer arithmetic */
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

/*
 * Compute a(n) = gcd(n!!, (n-1)!!) where n!! is the double factorial.
 * For n = 2m or 2m+1 the result equals A049606(m), the product of odd
 * primes ≤ m taken with exponent equal to the minimum of
 *   v_p(m!)   and   v_p((2m−1)!!)
 * for each odd prime p ≤ m.
 */
extern seq_t seq_mul(seq_t a, seq_t b);
extern seq_t seq_pow(seq_t base, idx_t exp);

seq_t a(idx_t n)
{
    /* The first terms are explicitly 1 */
    if (n <= 1)
        return (seq_t)1;

    /* Use m = floor(n/2) : a(2m) = a(2m+1) */
    idx_t m = n >> 1;                 /* integer division by 2 */
    if (m == 0)                        /* n == 0 or 1 handled already */
        return (seq_t)1;

    /* Sieve all primes ≤ m (m ≤ 5000 for n ≤ 10000) */
    size_t limit = (size_t)m;
    bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!is_prime)
        return (seq_t)0;              /* unlikely; safety net */

    for (size_t i = 0; i <= limit; ++i)
        is_prime[i] = true;
    if (limit >= 0)
        is_prime[0] = false;
    if (limit >= 1)
        is_prime[1] = false;

    for (size_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (size_t q = p * p; q <= limit; q += p)
                is_prime[q] = false;
        }
    }

    seq_t result = (seq_t)1;
    idx_t N = (2 * m) - 1;           /* (2m-1) used in odd double factorial */

    for (size_t ip = 2; ip <= limit; ++ip) {
        if (!is_prime[ip])
            continue;

        idx_t p = (idx_t)ip;
        if (p == 2)                  /* even prime contributes nothing */
            continue;

        /* exponent of p in m! */
        idx_t e1 = 0;
        idx_t t = m;
        while (t) {
            t /= p;
            e1 += t;
        }

        /* exponent of p in (2m-1)!! (odd double factorial) */
        idx_t e2 = 0;
        idx_t pk = p;
        while (pk <= N) {
            idx_t Ti = N / pk;                /* floor(N / p^k) */
            idx_t count = (Ti + 1) / 2;        /* number of odd multiples */
            e2 += count;
            /* safe to multiply because pk ≤ N and N < 2^64 for our limits */
            pk *= p;
        }

        idx_t e = e1 < e2 ? e1 : e2;
        if (e > 0) {
            seq_t powp = seq_pow((seq_t)p, e);
            result = seq_mul(result, powp);
        }
    }

    free(is_prime);
    return result;
}
