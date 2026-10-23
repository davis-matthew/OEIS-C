/*
 * A138447  a(n) = ((n-th prime)^6-(n-th prime)^2)/12.
 * https://oeis.org/A138447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((p_n)^6-(p_n)^2)/12 where p_n is the n-th prime (p_1 =
 *            2)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint32_t *primes = NULL;      /* dynamic array of primes */
static size_t   primes_cnt = 0;      /* number of primes stored */
static size_t   primes_cap = 0;      /* allocated size */

/* Ensure the array holds at least n primes (n >= 1). */
static void ensure_primes(idx_t n)
{
    if (primes_cnt >= (size_t)n) return;

    /* Grow the array if needed. */
    if (primes_cap < 1024) {
        primes_cap = 1024;
        primes = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
    }

    /* Generate primes by trial division until we have n primes. */
    uint32_t candidate = (primes_cnt == 0) ? 2 : primes[primes_cnt - 1] + 1;
    while (primes_cnt < (size_t)n) {
        int is_prime = 1;
        uint64_t sq = (uint64_t)candidate * (uint64_t)candidate;
        for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * (uint64_t)primes[i] <= sq; ++i) {
            if (candidate % primes[i] == 0) { is_prime = 0; break; }
        }
        if (is_prime) {
            if (primes_cnt == primes_cap) {
                primes_cap *= 2;
                primes = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
            }
            primes[primes_cnt++] = candidate;
        }
        ++candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for offset 1 */
    ensure_primes(n);
    uint32_t p = primes[n - 1];          /* p_n */
    seq_t p2 = (seq_t)p * p;             /* p^2 */
    seq_t p4 = p2 * p2;                  /* p^4 */
    seq_t p6 = p4 * p2;                  /* p^6 */
    return (p6 - p2) / 12;
}
