/*
 * A072356  a(n) = if n=p*q^2 for primes p<>q then q*p^2 else n.
 * https://oeis.org/A072356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n, unless n = p*q^2 with primes p ≠ q, in which case a(n)
 *            = q * p^2. a(1) = 1, a(2) = 2, …
 * Range:     u8 n <= 51, u16 n <= 763, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function below factors the index n into at most three prime factors
   and, if the prime factorisation has exactly two distinct primes with
   exponents 1 and 2, returns the swapped value.  All operations are
   performed with the supplied integer type seq_t, which may be 8, 16,
   32, 64 or 128 bits, or a big‑integer class. */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2)          /* a(1) = 1 */
        return (seq_t)n;

    uint64_t m   = (uint64_t)n;
    uint64_t primes[3] = {0};
    int       exps[3]  = {0};
    int       cnt = 0;

    /* factor 2 */
    if ((m & 1ULL) == 0) {
        uint64_t p = 2;
        int e = 0;
        while ((m & 1ULL) == 0) {
            m >>= 1;
            ++e;
        }
        primes[cnt] = p;
        exps[cnt]   = e;
        ++cnt;
    }

    /* factor odd primes */
    for (uint64_t d = 3; d*d <= m && cnt < 3; d += 2) {
        if (m % d == 0) {
            uint64_t p = d;
            int e = 0;
            while (m % d == 0) {
                m /= d;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt]   = e;
            ++cnt;
        }
    }

    /* remaining prime factor, if any */
    if (m > 1) {
        if (cnt >= 3)  /* too many distinct primes */
            return (seq_t)n;
        primes[cnt] = m;
        exps[cnt]   = 1;
        ++cnt;
    }

    /* we need exactly two distinct primes */
    if (cnt != 2)
        return (seq_t)n;

    uint64_t p, q;
    if (exps[0] == 1) {  /* first prime is exponent 1 */
        p = primes[0];
        q = primes[1];
    } else if (exps[1] == 1) {
        p = primes[1];
        q = primes[0];
    } else {
        /* both exponents are 2 or greater – not the required pattern */
        return (seq_t)n;
    }

    /* check that the other exponent is exactly 2 */
    int e_other = (exps[0] == 1) ? exps[1] : exps[0];
    if (e_other != 2)
        return (seq_t)n;

    /* compute q * p^2 using seq_t arithmetic */
    seq_t p2 = (seq_t)p * (seq_t)p;   /* p^2 */
    seq_t res = (seq_t)q * p2;         /* q * p^2 */
    return res;
}
