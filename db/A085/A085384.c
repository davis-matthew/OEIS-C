/*
 * A085384  Ramanujan sum c_n(4).
 * https://oeis.org/A085384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = phi(n) * mu(n / gcd(n,4)) / phi(n / gcd(n,4))
 * Range:     i8 n <= 2397, i16 n <= 9337, i32 n <= 9337, i64 n <= 9337,
 *            i128 n <= 9337, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Prime sieve for factorisation (used for φ and μ)                   */
/* ------------------------------------------------------------------ */

static const uint32_t MAX_PRIME = 100000;   /* enough for n up to 10^10 (sqrt ~ 100 000) */
static uint32_t *prime_list = NULL;
static size_t prime_count = 0;

static void init_primes(void)
{
    if (prime_list != NULL) return;       /* already initialised */

    bool *is_prime = (bool*)malloc((MAX_PRIME + 1) * sizeof(bool));
    if (!is_prime) abort();

    for (uint32_t i = 0; i <= MAX_PRIME; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (uint32_t i = 2; (uint64_t)i * i <= MAX_PRIME; ++i)
        if (is_prime[i])
            for (uint32_t j = i * i; j <= MAX_PRIME; j += i)
                is_prime[j] = false;

    /* Count primes and allocate list. */
    prime_count = 0;
    for (uint32_t i = 2; i <= MAX_PRIME; ++i)
        if (is_prime[i]) ++prime_count;

    prime_list = (uint32_t*)malloc(prime_count * sizeof(uint32_t));
    if (!prime_list) abort();

    size_t pos = 0;
    for (uint32_t i = 2; i <= MAX_PRIME; ++i)
        if (is_prime[i]) prime_list[pos++] = i;

    free(is_prime);
}

/* ------------------------------------------------------------------ */
/*   φ(n)  (Euler's totient)                                          */
/* ------------------------------------------------------------------ */

static inline seq_t phi_uint64(uint64_t n)
{
    seq_t result = (seq_t)n;
    uint64_t temp = n;

    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = prime_list[i];
        if ((uint64_t)p * p > temp) break;
        if (temp % p == 0) {
            result = result / p * (p - 1);
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) {                /* remaining prime factor */
        result = result / (seq_t)temp * ((seq_t)temp - 1);
    }
    return result;
}

/* ------------------------------------------------------------------ */
/*   μ(n)  (Möbius function)                                          */
/* ------------------------------------------------------------------ */

static inline int mu_uint64(uint64_t n)
{
    int mu = 1;
    uint64_t temp = n;

    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = prime_list[i];
        if ((uint64_t)p * p > temp) break;
        if (temp % p == 0) {
            int cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                ++cnt;
                if (cnt > 1) return 0;      /* square factor → μ = 0 */
            }
            mu = -mu;
        }
    }
    if (temp > 1) mu = -mu;                /* remaining prime factor */
    return mu;
}

/* ------------------------------------------------------------------ */
/*   a(n)  (Ramanujan sum c_n(4))                                    */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    init_primes();                    /* Ensure the prime list is ready */

    uint64_t N = (uint64_t)n;
    uint64_t g;

    /* gcd(n,4) handled by simple tests */
    if ((N & 3u) == 0u)      g = 4;
    else if ((N & 1u) == 0u) g = 2;
    else                     g = 1;

    uint64_t m = N / g;                /* n / gcd(n,4) */

    int mu = mu_uint64(m);
    if (mu == 0) return (seq_t)0;       /* μ = 0 ⇒ term is zero */

    seq_t phiN = phi_uint64(N);
    seq_t phiM = phi_uint64(m);

    seq_t result = phiN / phiM;        /* exact division */
    if (mu < 0) result = -result;      /* sign given by μ(m) */

    return result;
}
