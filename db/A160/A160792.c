/*
 * A160792  Vertex number of a rectangular spiral related to prime numbers.
 *          The distances between nearest edges of the spiral that are
 *          parallel to the initial edge are the prime numbers, while the
 *          distances between nearest edges perpendicular to the initial edge
 *          are all one.
 * https://oeis.org/A160792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + (n is even ? sum_{j=1}^{n/2} prime(j) : n/2+1),
 *            a(0) = 0, a(1) = 1 Implementation uses a simple sieve of
 *            accumulated primes
 * Range:     u8 n <= 15, u16 n <= 93, u32 n <= 3023, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* Prime storage ----------------------------------------------------- */
static uint32_t *primes = NULL;          /* primes[i] is the (i+1)-th prime */
static seq_t   *prime_sum = NULL;        /* prime_sum[i] = sum of first i primes, prime_sum[0] = 0 */
static size_t   primes_cap = 0;          /* allocated capacity of primes array */
static size_t   primes_cnt = 0;          /* number of primes currently stored */

/* Append a new prime and update the prefix‑sum array. */
static void push_prime(uint32_t p)
{
    if (primes_cnt + 1 > primes_cap) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 8;
        uint32_t *tmp_p  = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
        seq_t   *tmp_ps = (seq_t   *)realloc(prime_sum, (new_cap + 1) * sizeof(seq_t));
        if (!tmp_p || !tmp_ps) return; /* allocation failure -- best effort */
        primes = tmp_p;
        prime_sum = tmp_ps;
        primes_cap = new_cap;
        /* prime_sum[0] is already 0, no need to reset */
    }
    primes[primes_cnt] = p;
    prime_sum[primes_cnt + 1] = prime_sum[primes_cnt] + (seq_t)p;
    ++primes_cnt;
}

/* Ensure that at least `needed` primes have been generated. */
static void ensure_primes(size_t needed)
{
    if (primes_cnt >= needed) return;

    uint32_t candidate = (primes_cnt == 0) ? 2 : primes[primes_cnt - 1] + 1;
    while (primes_cnt < needed) {
        bool is_prime = true;
        for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= candidate; ++i) {
            if (candidate % primes[i] == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) push_prime(candidate);
        ++candidate;
    }
}

/* ----- Public sequence function ----------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Number of primes needed for n: floor(n/2) is enough, add one to be safe. */
    size_t needed = (size_t)(n / 2) + 1;
    ensure_primes(needed);

    seq_t val = (seq_t)1; /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        if ((i & 1) == 0) {                 /* even index i = 2k */
            size_t k = (size_t)(i >> 1);
            val += prime_sum[k];
        } else {                            /* odd index i = 2k+1 */
            size_t k = (size_t)((i - 1) >> 1);
            val += (seq_t)(k + 1);
        }
    }
    return val;
}
