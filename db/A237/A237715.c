/*
 * A237715  Number of ordered ways to write n = p + q (q > 0) with p, prime(p)
 *          - p + 1 and prime(prime(q)) - prime(q) + 1 all prime.
 * https://oeis.org/A237715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered ways to write n = p + q (q>0) where p
 *            is prime, prime(p)-p+1 is prime, and prime(prime(q))-prime(q)+1
 *            is prime
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define SIEVE_LIMIT 2000000 /* must cover prime(104729) ≈ 1.3 M */

static unsigned char is_prime_arr[SIEVE_LIMIT + 1];
static uint32_t primes_array[200000];
static size_t primes_cnt = 0;
static bool primes_ready = false;

/* Build the prime table up to SIEVE_LIMIT. */
static void build_primes(void)
{
    if (primes_ready) return;
    /* sieve of Eratosthenes */
    for (size_t i = 0; i <= SIEVE_LIMIT; ++i) is_prime_arr[i] = 1;
    is_prime_arr[0] = is_prime_arr[1] = 0;
    for (size_t i = 2; i * i <= SIEVE_LIMIT; ++i)
        if (is_prime_arr[i])
            for (size_t j = i * i; j <= SIEVE_LIMIT; j += i)
                is_prime_arr[j] = 0;

    /* collect primes into array */
    size_t idx = 0;
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (is_prime_arr[i])
            primes_array[idx++] = (uint32_t)i;
    primes_cnt = idx;
    primes_ready = true;
}

seq_t a(idx_t n)
{
    build_primes();                     /* ensure primes are ready */
    if (n < 2) return 0;                /* a(1)=0, a(2)=0 by definition */

    seq_t count = 0;
    for (size_t ip = 0; ip < primes_cnt; ++ip)
    {
        uint32_t p = primes_array[ip];          /* p is prime, p ≤ n−1 */
        if (p >= (uint32_t)n) break;           /* stop when p > n−1 */

        uint32_t prime_p   = primes_array[p - 1];
        uint32_t val1      = prime_p - p + 1;
        if (val1 > SIEVE_LIMIT || !is_prime_arr[val1]) continue;

        idx_t q = n - p;                       /* q > 0 automatically */
        if (q == 0) continue;
        uint32_t prime_q       = primes_array[q - 1];
        uint32_t prime_prime_q = primes_array[prime_q - 1];
        uint32_t val2          = prime_prime_q - prime_q + 1;
        if (val2 > SIEVE_LIMIT || !is_prime_arr[val2]) continue;

        ++count;                               /* found a valid way */
    }
    return count;
}
