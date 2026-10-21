/*
 * A056676  Number of non-unitary but squarefree divisors of
 *          binomial(n,floor(n/2)). Also number of nonsquarefree but unitary
 *          divisors of binomial(n,floor(n/2)).
 * https://oeis.org/A056676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^r – 2^r1 * where r = number of distinct primes dividing
 *            C(n,⌊n/2⌋) * r1 = number of those primes whose exponent in the
 *            binomial is exactly 1
 * Range:     u8 n <= 34, u16 n <= 88, u32 n <= 206, u64 n <= 488,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include generic utilities. */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/pow2.h"

/* number of times p divides m!  */
static void exponent_in_factorial(idx_t m, idx_t p, idx_t *e)
{
    *e = 0;
    idx_t power = p;
    while (power <= m) {
        *e += m / power;
        /* avoid overflow when power*p > m */
        if (power > m / p) break;
        power *= p;
    }
}

/* exponent of p in C(n,k) */
static idx_t exponent_in_binomial(idx_t n, idx_t k, idx_t p)
{
    idx_t e_n, e_k, e_nk;
    exponent_in_factorial(n, p, &e_n);
    exponent_in_factorial(k, p, &e_k);
    exponent_in_factorial(n - k, p, &e_nk);
    return e_n - e_k - e_nk;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1, return 0 */

    idx_t k = n / 2;               /* floor(n/2) */

    /* Obtain all primes ≤ n. */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(n, &primes, &prime_cnt);

    idx_t distinct_primes = 0;    /* r  */
    idx_t primes_with_exp1 = 0;   /* r1 */

    /* For each prime, compute its exponent in the binomial coefficient. */
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = primes[i];
        idx_t e = exponent_in_binomial(n, k, p);
        if (e == 0) continue;     /* prime does not divide the binomial */
        ++distinct_primes;
        if (e == 1) ++primes_with_exp1;
    }

    free(primes);                 /* memory allocated by build_primes_upto */

    /* Result: 2^r – 2^r1 */
    seq_t total = pow2(distinct_primes);
    seq_t unitary = pow2(primes_with_exp1);
    return total - unitary;
}
