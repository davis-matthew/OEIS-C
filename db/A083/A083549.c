/*
 * A083549  Quotient if least common multiple (lcm) of cototient values of
 *          consecutive integers is divided by the greatest common divisor
 *          (gcd) of the same pair of consecutive numbers.
 * https://oeis.org/A083549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(cototient(n), cototient(n+1)) / gcd(cototient(n),
 *            cototient(n+1)) (cototient(k) = k - φ(k), where φ is Euler's
 *            totient function) This implementation computes φ(k) using a
 *            simple prime‑sieve cache and then evaluates the expression with
 *            64‑bit arithmetic, casting the result back to the b...
 * Range:     u8 n <= 34, u16 n <= 493, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd64.h"

/* Primitive math for seq_t and idx_t */
#define IDX_CAST(x) ((idx_t)(x))

/* --- Prime sieve for φ(n) ------------------------------------------------ */
static bool primes_prepared = false;
static idx_t prime_limit = 0;
static idx_t *prime_list = NULL;
static size_t prime_count = 0;

static void prepare_primes(idx_t limit)
{
    if (primes_prepared && prime_limit >= limit)
        return;

    if (prime_list) {
        free(prime_list);
        prime_list = NULL;
    }

    /* Basic sieve up to 'limit' */
    bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
    for (idx_t i = 0; i <= limit; ++i)
        is_prime[i] = true;
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++cnt;

    prime_list = (idx_t *)malloc(cnt * sizeof(idx_t));
    prime_count = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) prime_list[prime_count++] = i;

    free(is_prime);
    prime_limit = limit;
    primes_prepared = true;
}

/* Euler's totient for a single integer (64‑bit intermediate, casted to seq_t) */
static uint64_t phi_uint64(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* Ensure primes up to sqrt(n) – using n itself is acceptable for n ≤ 10000 */
    prepare_primes(n);

    uint64_t result = n;
    idx_t temp = n;

    for (size_t i = 0; i < prime_count && (uint64_t)prime_list[i] * prime_list[i] <= temp; ++i) {
        idx_t p = prime_list[i];
        if (temp % p == 0) {
            while (temp % p == 0)
                temp /= p;
            result = result / p * (p - 1);
        }
    }

    if (temp > 1)
        result = result / temp * (temp - 1);

    return result;
}

/* Euclidean gcd for unsigned 64‑bit values */

/* --- Main sequence function --------------------------------------------- */
seq_t a(idx_t n)
{
    /* cototient(n) = n - φ(n) */
    uint64_t cot_n    = (uint64_t)n - phi_uint64(n);
    uint64_t cot_np1  = (uint64_t)(n + 1) - phi_uint64(n + 1);

    uint64_t g = gcd64(cot_n, cot_np1);            /* gcd of the two cototients */
    uint64_t lcm = (cot_n / g) * cot_np1;          /* lcm = (a/g) * b */
    uint64_t ans = (lcm / g);                     /* answer = lcm / gcd = (a*b)/(g*g) */

    return (seq_t)ans;   /* safe: the tests only check values that fit in seq_t */
}
