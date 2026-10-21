/*
 * A083900  Number of divisors of n with largest digit <= 5 (base 10).
 * https://oeis.org/A083900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of divisors of n whose decimal digits are all <= 5
 *            (base 10)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   digits_leq5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/digits_leq5.h"

/* Factorize n (uint64_t) into prime factors.
   primes[]   : array to store prime factors (max 64)
   exps[]     : array to store matching exponents
   cnt        : number of distinct prime factors found (output) */
static void factor_uint64(uint64_t n, uint64_t primes[], int exps[], int *cnt)
{
    int pos = 0;

    if (n == 0) {
        *cnt = 0;
        return;
    }

    if ((n & 1) == 0) {
        int e = 0;
        while ((n & 1) == 0) {
            n >>= 1;
            e++;
        }
        primes[pos] = 2;
        exps[pos]   = e;
        pos++;
    }

    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) {
                n /= p;
                e++;
            }
            primes[pos] = p;
            exps[pos]   = e;
            pos++;
        }
    }

    if (n > 1) {
        primes[pos] = n;
        exps[pos]   = 1;
        pos++;
    }

    *cnt = pos;
}

/* Recursive enumeration of all divisors given the prime factorization.
   idx   : current index in primes[]
   k     : total number of distinct primes
   curr  : product of already chosen prime powers
   result: pointer to the running count of qualifying divisors */
static void enumerate_divisors(int idx, int k,
                               const uint64_t primes[],
                               const int exps[],
                               seq_t curr,
                               seq_t *result)
{
    if (idx == k) {
        if (digits_leq5(curr))
            (*result)++;
        return;
    }

    seq_t val = curr;
    for (int e = 0; e <= exps[idx]; e++) {
        enumerate_divisors(idx + 1, k, primes, exps, val, result);
        val *= (seq_t)primes[idx];          /* prepare for next exponent */
    }
}

/* Public sequence function.  Computes a(n) for n >= 1.  */
seq_t a(idx_t n)
{
    if (n == 0)  /* behavior for n < 1 is undefined; return 0 */
        return (seq_t)0;

    uint64_t primes[64];
    int     exps[64];
    int     cnt;

    factor_uint64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)0;
    enumerate_divisors(0, cnt, primes, exps, (seq_t)1, &result);

    return result;
}
