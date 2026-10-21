/*
 * A090881  Suppose n=(p1^e1)(p2^e2)... where p1,p2,... are the prime numbers
 *          and e1,e2,... are nonnegative integers. Then a(n) = e1 + (e2)*4 +
 *          (e3)*16 + (e4)*64 + ... + (ek)*(4^(k-1)) + ...
 * https://oeis.org/A090881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{k≥1} e_k · 4^(k‑1) where e_k is the exponent of the
 *            k‑th prime in the * factorisation of n. (e_1 is for 2, e_2 for
 *            3, e_3 for 5, …)
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   factor_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_uint64.h"

/* Simple dynamic list of primes up to the maximum n seen so far. */
static uint64_t *prime_list   = NULL;
static size_t    prime_cnt    = 0;
static uint64_t  prime_max    = 0;

/* Generate all primes ≤ limit using a classic sieve of Eratosthenes. */
static void generate_primes(uint64_t limit)
{
    if (limit <= prime_max) return;            /* already have enough primes */

    uint8_t *sieve = (uint8_t*)malloc((size_t)(limit + 1));
    if (!sieve) return;
    for (uint64_t i = 0; i <= limit; ++i) sieve[i] = 1;
    if (limit >= 0) sieve[0] = 0;
    if (limit >= 1) sieve[1] = 0;

    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (sieve[i]) {
            for (uint64_t j = i * i; j <= limit; j += i) sieve[j] = 0;
        }
    }

    /* count primes and allocate the list */
    size_t cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i) if (sieve[i]) ++cnt;
    uint64_t *new_list = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!new_list) { free(sieve); return; }

    size_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i) if (sieve[i]) new_list[idx++] = i;
    free(sieve);
    free(prime_list);
    prime_list = new_list;
    prime_cnt  = cnt;
    prime_max  = limit;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;   /* a(1) = 0 */

    generate_primes(n);            /* ensure we have all primes up to n */

    uint64_t fac[32];              /* primes dividing n (ascending) */
    int      exp[32];              /* their exponents          */
    int      fac_cnt;              /* number of distinct primes */
    factor_uint64((uint64_t)n, fac, exp, &fac_cnt);

    seq_t result = 0;
    seq_t weight = 1;              /* weight = 4^(k‑1) for the current prime k */
    int j = 0;                     /* index into the factor list */

    for (size_t i = 0; i < prime_cnt && prime_list[i] <= n; ++i) {
        if (j < fac_cnt && prime_list[i] == fac[j]) {
            result += weight * (seq_t)exp[j];
            ++j;
        }
        weight <<= 2;              /* multiply by 4 for the next prime */
    }

    return result;
}
