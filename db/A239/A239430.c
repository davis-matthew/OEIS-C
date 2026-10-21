/*
 * A239430  Number of ways to write n = k + m with k > 0 and m > 0 such that
 *          pi(2*k) - pi(k) is prime and pi(2*m) - pi(m) is a square, where
 *          pi(x) denotes the number of primes not exceeding x.
 * https://oeis.org/A239430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered pairs (k,m) with k>0, m>0, k+m=n such
 *            that pi(2k)-pi(k) is prime and pi(2m)-pi(m) is a perfect square
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*----------- local helpers --------------------------------------------------*/
static bool is_prime_uint64(uint64_t x)
{
    if (x < 2) return false;
    for (uint64_t d = 2; d*d <= x; ++d)
        if (x % d == 0) return false;
    return true;
}

static uint64_t isqrt_uint64(uint64_t x)
{
    uint64_t r = 0, bit = 1ULL << 62; /* highest power of four <= x */
    while (bit > x) bit >>= 2;
    while (bit != 0) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/*----------- π-function cache --------------------------------------------*/
static idx_t pi_cache_limit = 0;
static uint32_t *pi_cache = NULL;

static void init_pi_cache(idx_t limit)
{
    if (limit <= pi_cache_limit) return;

    uint32_t *new_pi = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    if (!new_pi) exit(1);

    unsigned char *is_prime = (unsigned char *)calloc(limit + 1, sizeof(unsigned char));
    if (!is_prime) {
        free(new_pi);
        exit(1);
    }

    for (idx_t i = 2; i <= limit; ++i) is_prime[i] = 1;

    for (idx_t p = 2; p * p <= limit; ++p)
        if (is_prime[p])
            for (idx_t j = p * p; j <= limit; j += p)
                is_prime[j] = 0;

    new_pi[0] = 0;
    for (idx_t i = 1; i <= limit; ++i)
        new_pi[i] = new_pi[i - 1] + (uint32_t)is_prime[i];

    free(is_prime);
    free(pi_cache);
    pi_cache = new_pi;
    pi_cache_limit = limit;
}

/*----------- main sequence function ------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    idx_t lim = n + n;            /* need pi up to 2*n */
    init_pi_cache(lim);

    seq_t count = 0;
    for (idx_t k = 1; k < n; ++k) {
        idx_t m = n - k;

        uint64_t diff_k = (uint64_t)pi_cache[2 * k] - (uint64_t)pi_cache[k];
        if (!is_prime_uint64(diff_k)) continue;

        uint64_t diff_m = (uint64_t)pi_cache[2 * m] - (uint64_t)pi_cache[m];
        uint64_t sqrt_m = isqrt_uint64(diff_m);
        if (sqrt_m * sqrt_m != diff_m) continue;

        ++count;
    }
    return count;
}
