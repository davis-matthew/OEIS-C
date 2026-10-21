/*
 * A059333  For 0<=A, 0<=B, n is an A-almost prime; m is a B-almost prime, k =
 *          n+m, k is a C-almost prime; a(n) = smallest number m such that
 *          A+B=C.
 * https://oeis.org/A059333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = least m ≥ 1 such that Ω(n) + Ω(m) = Ω(n + m)
 * Range:     u8 n <= 31, u16 n <= 767, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* local implementation detail: we need a list of small primes.
   The size chosen (1e6) is more than enough for n ≤ 10 000
   and any m obtained in the search (they are all < 1e6). */
static int primes_limit = 1000000;
static uint32_t *primes = NULL;
static size_t primes_count = 0;
static int primes_sieved = 0;

/* Simple sieve of Eratosthenes, stores primes in the global array */
static void sieve_primes(void)
{
    if (primes_sieved) return;
    int *is_composite = (int *)calloc(primes_limit + 1, sizeof(int));
    size_t count = 0;
    for (int i = 2; i <= primes_limit; ++i) {
        if (!is_composite[i]) {
            if (count == 0) {
                primes = (uint32_t *)malloc(sizeof(uint32_t) * primes_limit);
            } else {
                primes = (uint32_t *)realloc(primes, sizeof(uint32_t) * (count + primes_limit));
            }
            primes[count++] = (uint32_t)i;
            if ((int64_t)i * i <= primes_limit) {
                for (int j = i * i; j <= primes_limit; j += i) {
                    is_composite[j] = 1;
                }
            }
        }
    }
    primes_count = count;
    primes_sieved = 1;
    free(is_composite);
}

/* Count prime factors with multiplicity of x (x ≥ 1) using the primes list. */
static unsigned omega_uint64(uint64_t x)
{
    if (x == 1) return 0;
    unsigned cnt = 0;
    for (size_t i = 0; i < primes_count; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * p > x) break;
        while (x % p == 0) {
            ++cnt;
            x /= p;
        }
    }
    if (x > 1) ++cnt; /* remaining prime factor */
    return cnt;
}

seq_t a(idx_t n)
{
    /* ensure primes are ready */
    if (!primes_sieved) sieve_primes();

    if (n == 0) return (seq_t)0; /* undefined case, return 0 */

    uint64_t n64 = (uint64_t)n;          /* n is at most uint64_t */
    unsigned omega_n = omega_uint64(n64);

    for (uint64_t m = 1;; ++m) {
        unsigned omega_m = omega_uint64(m);
        uint64_t k = n64 + m;
        unsigned omega_k = omega_uint64(k);
        if (omega_n + omega_m == omega_k)
            return (seq_t)m;
    }
}
