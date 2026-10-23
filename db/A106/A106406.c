/*
 * A106406  Expansion of (eta(q) * eta(q^15))^2 / (eta(q^3) * eta(q^5)) in
 *          powers of q.
 * https://oeis.org/A106406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) is multiplicative with prime powers a(3^e) = a(5^e) =
 *            (-1)^e a(p^e) = 1 if p ≡ 7,11,13,14 (mod 15) and e is even, 0 if
 *            p ≡ 7,11,13,14 (mod 15) and e is odd, e+1 if p ≡ 1, 4 (mod 15),
 *            (e+1)*(-1)^e if p ≡ 2, 8 (mod 15), a(n) = ∏_p a(p^e) over the
 *            prime factorisation of n. a(1) = 1
 * Range:     i8 n <= 79, i16 n <= 79, i32 n <= 79, i64 n <= 79, i128 n <= 79,
 *            big any n
 * Helpers:   prime_power_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/prime_power_factor.h"

static bool  *sieve_used = NULL;
static int   sieve_size = 0;
static int   *prime_list = NULL;
static int   prime_count = 0;
static int   prime_cap = 0;

/* Ensure the sieve contains all primes up to 'limit' (inclusive) */
static void ensure_primes(int limit)
{
    if (limit <= sieve_size) return;

    if (sieve_used) free(sieve_used);
    if (prime_list) free(prime_list);

    sieve_size = limit + 1;
    sieve_used = (bool*)malloc(sieve_size);
    for (int i = 0; i < sieve_size; ++i) sieve_used[i] = false;

    prime_cap = limit / 10 + 10;
    prime_list = (int*)malloc(prime_cap * sizeof(int));
    prime_count = 0;

    for (int i = 2; i <= limit; ++i) {
        if (!sieve_used[i]) {
            if (prime_count == prime_cap) {
                prime_cap <<= 1;
                prime_list = (int*)realloc(prime_list, prime_cap * sizeof(int));
            }
            prime_list[prime_count++] = i;
            if ((int64_t)i * i <= limit) {
                for (int j = i * i; j <= limit; j += i)
                    sieve_used[j] = true;
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset starts at 1, but handle 0 gracefully */
    seq_t result = 1;
    idx_t temp = n;

    /* Determine size needed for sieve */
    int max_limit = 2;
    while ((idx_t)max_limit * (idx_t)max_limit <= temp) {
        max_limit++;
    }
    ensure_primes(max_limit);

    /* Factorise n */
    for (int i = 0; i < prime_count; ++i) {
        int p = prime_list[i];
        idx_t pp = (idx_t)p * (idx_t)p;
        if (pp > temp) break;
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            seq_t f = prime_power_factor(p, e);
            if (f == 0) return 0;
            result *= f;
        }
    }

    /* Remaining prime factor, if any */
    if (temp > 1) {
        /* temp is prime */
        seq_t f = prime_power_factor((int)temp, 1);
        if (f == 0) return 0;
        result *= f;
    }

    return result;
}
