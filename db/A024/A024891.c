/*
 * A024891  s(1)t(n) + s(2)t(n-1) + ... + s(k)t(n-k+1), where k = [ n/2 ], s =
 *          A023531, t = (primes).
 * https://oeis.org/A024891
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1}^{⌊n/2⌋} s(i) * p(n-i+1) where p(j) is the j‑th
 *            prime (p(1)=2) and s(i) = 1 iff i = k(k+3)/2 for some integer k
 *            ≥ 1, otherwise 0
 * Range:     u8 n <= 27, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* limits (large enough for n up to about 20000) */
#define MAX_K          20000   /* maximum i needed (≈ n/2)       */
#define MAX_PRIME_CNT  20000   /* number of primes to precompute */
#define SIEVE_LIMIT    250000  /* upper bound to get enough primes */

/* static storage */
static uint64_t primes[MAX_PRIME_CNT];
static size_t prime_cnt = 0;
static bool s_flag[MAX_K + 1];
static bool data_inited = false;

/* precompute s_flag and prime list */
static void init_data(void)
{
    if (data_inited) return;

    /* initialise s_flag with the sequence i = k(k+3)/2 */
    for (size_t i = 0; i <= MAX_K; ++i) s_flag[i] = false;
    for (size_t k = 1; ; ++k) {
        size_t v = k * (k + 3) / 2;
        if (v > MAX_K) break;
        s_flag[v] = true;
    }

    /* sieve up to SIEVE_LIMIT to collect at least MAX_PRIME_CNT primes */
    bool *is_prime = (bool*)malloc((SIEVE_LIMIT + 1) * sizeof(bool));
    if (!is_prime) abort();   /* allocation failure, terminate */

    for (size_t i = 0; i <= SIEVE_LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (size_t i = 2; i * i <= SIEVE_LIMIT; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= SIEVE_LIMIT; j += i) {
                is_prime[j] = false;
            }
        }
    }
    for (size_t i = 2; i <= SIEVE_LIMIT && prime_cnt < MAX_PRIME_CNT; ++i) {
        if (is_prime[i]) primes[prime_cnt++] = (uint64_t)i;
    }

    free(is_prime);
    data_inited = true;
}

/* main sequence routine -------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* sequence defined for n >= 2 only */

    init_data();

    idx_t k = n / 2;              /* floor(n/2) */
    seq_t result = 0;

    for (idx_t i = 1; i <= k; ++i) {
        if (s_flag[(size_t)i]) {
            size_t prime_index = (size_t)(n - i);   /* t(n-i+1) -> primes[n-i] */
            if (prime_index < prime_cnt) {
                result += (seq_t)primes[prime_index];
            }
        }
    }
    return result;
}
