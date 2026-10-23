/*
 * A158077  Denominator of Bernoulli(n, 1/6).
 * https://oeis.org/A158077
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 2^(n - n%2) * 3^n * Π_{p prime, p>3, (p-1) | n} p
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   mul_small, pow_seq, seq_mul, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          (with a(0) = 1) */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_one.h"
#include "../utils/seq_mul.h"
#include "../utils/mul_small.h"
#include "../utils/pow_seq.h"

static uint64_t *prime_list = NULL;   /* array of primes ≤ current max */
static size_t   prime_cnt   = 0;      /* number of primes in the list   */
static idx_t    prime_max   = 1;      /* largest prime precomputed      */

/* Ensure primes up to limit are available in prime_list */
static void
ensure_primes(idx_t limit)
{
    if (limit <= prime_max) return;

    /* Allocate boolean array for sieve */
    size_t sz = (size_t)limit + 1;
    unsigned char *is_prime = (unsigned char *)malloc(sz);
    if (!is_prime) return;
    for (size_t i = 0; i < sz; ++i) is_prime[i] = 1;
    if (sz > 0) is_prime[0] = 0;
    if (sz > 1) is_prime[1] = 0;

    for (size_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= (size_t)limit; j += i)
                is_prime[j] = 0;
        }
    }

    /* Count primes to allocate array */
    size_t cnt = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i)
        if (is_prime[i]) ++cnt;

    uint64_t *new_list = (uint64_t *)realloc(prime_list, cnt * sizeof(uint64_t));
    if (!new_list) {
        free(is_prime);
        return;
    }
    prime_list = new_list;
    size_t idx = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i)
        if (is_prime[i]) prime_list[idx++] = i;
    prime_cnt = cnt;
    prime_max = limit;

    free(is_prime);
}

seq_t a(idx_t n)
{
    if (n == 0) return seq_one();

    /* Compute powers of 2 and 3 */
    int exp2 = (int)(n - (n & 1ULL));            /* n - n%2 */
    int exp3 = (int)n;
    seq_t pow2 = pow_seq((seq_t)2, exp2);
    seq_t pow3 = pow_seq((seq_t)3, exp3);
    seq_t result = seq_mul(pow2, pow3);

    /* Prepare primes up to n+1 */
    ensure_primes(n + 1);

    /* Multiply by primes p>3 with (p-1) | n */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p <= 3) continue;
        if ((n % (p - 1)) == 0)
            result = mul_small(result, (unsigned int)p);
    }

    return result;
}
