/*
 * A156034  Denominator of (Sum_{k=1..n} k^3)/n!.
 * https://oeis.org/A156034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of ((Sum_{k=1..n} k^3)/n!) Note: Sum_{k=1..n}
 *            k^3 = (n(n+1)/2)^2. The fraction is (n(n+1)/2)^2 / n! and the
 *            denominator equals ∏_{p≤n} p^{max(0, e_den(p) – 2·e_T(p))},
 *            where e_den(p) is the exponent of prime p in n! and e_T(p) is
 *            the exponent of p in n(n+1)/2
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

/* cached primes up to the largest n requested */
static uint64_t *prime_list = NULL;
static size_t    prime_cnt  = 0;
static idx_t     prime_max  = 0;

/* ensure prime_list contains all primes ≤ n */
static void
ensure_primes(idx_t n)
{
    if (n <= prime_max)
        return;

    if (prime_list != NULL)
        free(prime_list);

    size_t cnt;
    uint64_t *tmp;
    build_primes_upto(n, &tmp, &cnt);

    prime_list = tmp;
    prime_cnt  = cnt;
    prime_max  = n;
}

/* fast exponentiation for seq_t */
static seq_t
pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp != 0) {
        if (exp & 1)
            result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* trivial cases */
    if (n < 2)
        return (seq_t)1;

    /* ensure we have primes up to n */
    ensure_primes(n);

    seq_t denom = 1;

    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        uint64_t p = prime_list[pi];

        /* exponent of p in n! (Legendre's formula) */
        idx_t e_den = 0;
        idx_t temp = n;
        while (temp) {
            temp /= p;
            e_den += temp;
        }

        /* exponent of p in n */
        idx_t e_n = 0;
        uint64_t val = n;
        while (val % p == 0) {
            ++e_n;
            val /= p;
        }

        /* exponent of p in n+1 */
        idx_t e_np1 = 0;
        val = n + 1;
        while (val % p == 0) {
            ++e_np1;
            val /= p;
        }

        /* exponent of p in T = n(n+1)/2 */
        idx_t e_T = e_n + e_np1;
        if (p == 2)
            e_T -= 1;                    /* division by 2 */

        /* exponent of p in the numerator N = T^2 */
        idx_t e_num = 2 * e_T;

        /* remaining exponent in the reduced denominator */
        idx_t e_final = (e_den > e_num) ? (e_den - e_num) : 0;
        if (e_final > 0) {
            denom = denom * pow_seq((seq_t)p, e_final);
        }
    }

    return denom;
}
