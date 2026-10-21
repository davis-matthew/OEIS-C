/*
 * A095175  Denominator of b(n) given by b(1) = 1, b(2) = 2; for n >= 3, b(n)
 *          = (-1)^n (2n-1) ((n-2)!!)^2/((n-1)!!)^2, where n!! is the double
 *          factorial A006882.
 * https://oeis.org/A095175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 18, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   exp_double_fact, exp_factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A095175
   a(n) = denominator of b(n) where 
   b(1) = 1, b(2) = 2, and for n >= 3 
   b(n) = (-1)^n (2n-1) ((n-2)!!)² / ((n-1)!!)² .
   The denominator is taken after reducing the fraction to lowest terms. 
*/

#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "../utils/exp_double_fact.h"
#include "../utils/exp_factorial.h"

static int primes[25000];
static int prime_count = 0;
static bool primes_ready = false;

/* Simple sieve to generate all primes up to 20000 (covers 2*n-1 for n <= 10000) */
static void init_primes(void)
{
    const int LIMIT = 20000;
    bool is_prime[LIMIT + 1];
    memset(is_prime, true, sizeof(is_prime));
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (int j = i * i; j <= LIMIT; j += i)
                is_prime[j] = false;
        }
    }
    for (int i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) primes[prime_count++] = i;
    primes_ready = true;
}

/* Integer power with seq_t result: base^exp (exp >= 0) */
static seq_t pow_int(int base, int exp)
{
    seq_t result = (seq_t)1;
    seq_t b = (seq_t)base;
    while (exp) {
        if (exp & 1)
            result = result * b;
        b = b * b;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (!primes_ready)
        init_primes();

    if (n <= 2)
        return (seq_t)1;

    seq_t denom = (seq_t)1;
    int two_n_minus_one = (int)(2 * n - 1);

    /* iterate over all primes <= 2*n-1 */
    for (int pi = 0; pi < prime_count && primes[pi] <= two_n_minus_one; ++pi) {
        int p = primes[pi];

        /* exponent of p in numerator */
        int e_num = 0;
        e_num += 2 * exp_double_fact((int)(n - 2), p);

        /* factor (2n-1) */
        int t = two_n_minus_one;
        while (t % p == 0) {
            e_num += 1;
            t /= p;
        }

        /* exponent of p in denominator */
        int e_den = 2 * exp_double_fact((int)(n - 1), p);

        int diff = e_num - e_den;
        if (diff < 0) {
            denom = denom * pow_int(p, -diff);
        }
    }

    return denom;
}
