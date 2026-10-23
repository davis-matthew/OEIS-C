/*
 * A171937  Forward van Eck transform of A000005.
 * https://oeis.org/A171937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A079427(n) – n where A079427(n) is the smallest m > n
 *            with d(m) = d(n) (d = number‑of‑divisors function). For n = 1
 *            the answer is 0 because τ(1)=1 occurs only at 1
 * Range:     u8 n <= 63, u16 n <= 4095, u32 n <= 8204, u64 n <= 8204,
 *            u128 n <= 8204, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static int primes_init = 0;
static uint64_t *primes = NULL;
static size_t primes_cnt = 0;

static void init_primes(void)
{
    if (primes_init) return;
    const uint64_t LIMIT = 1000000;          /* primes up to 1 000 000 (8 MB memory) */
    unsigned char *is_prime = (unsigned char*)malloc((LIMIT + 1) * sizeof(unsigned char));
    for (uint64_t i = 0; i <= LIMIT; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (uint64_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (uint64_t j = i * i; j <= LIMIT; j += i) is_prime[j] = 0;
        }
    }
    size_t cnt = 0;
    for (uint64_t i = 2; i <= LIMIT; ++i) if (is_prime[i]) ++cnt;
    primes = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    size_t idx = 0;
    for (uint64_t i = 2; i <= LIMIT; ++i) if (is_prime[i]) primes[idx++] = i;
    free(is_prime);
    primes_cnt = cnt;
    primes_init = 1;
}

static inline seq_t tau_of(idx_t n)
{
    if (n == 0) return 0;
    seq_t result = 1;
    uint64_t tmp = n;
    for (size_t pi = 0; pi < primes_cnt && primes[pi] * primes[pi] <= tmp; ++pi) {
        uint64_t p = primes[pi];
        if (tmp % p == 0) {
            int exp = 0;
            do { tmp /= p; ++exp; } while (tmp % p == 0);
            result *= (seq_t)(exp + 1);
        }
    }
    if (tmp > 1) result *= 2;   /* remaining prime factor */
    return result;
}

seq_t a(idx_t n)
{
    init_primes();                /* ensure primes are ready */
    seq_t target = tau_of(n);
    if (target == 1) return 0;    /* τ(1)=1 occurs only at n=1 */
    for (idx_t m = n + 1;; ++m) {
        if (tau_of(m) == target) return (seq_t)(m - n);
    }
}
