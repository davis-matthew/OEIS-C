/*
 * A105561  a(n) is the m-th prime, where m is the number of distinct prime
 *          factors of n (A001221), a(1) = 1.
 * https://oeis.org/A105561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the m-th prime, where m = A001221(n), a(1) = 1
 * Range:     u8 n <= 120, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   count_distinct_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/count_distinct_prime_factors.h"

/* storage for primes */
#define PRIME_CAP 1000                      /* enough for all expected m values   */
static uint32_t primes[PRIME_CAP];
static size_t prime_cnt = 0;

/* ensure primes array has at least 'need' primes (1‑based) */
static void ensure_primes(size_t need)
{
    if (prime_cnt >= need)
        return;
    if (prime_cnt == 0) {
        primes[prime_cnt++] = 2;
    }
    uint32_t cand = primes[prime_cnt - 1] + 1;
    if ((cand & 1U) == 0)
        cand++;               /* start with odd number */
    while (prime_cnt < need) {
        int is_prime = 1;
        for (size_t i = 0; i < prime_cnt &&
             (uint64_t)primes[i] * primes[i] <= cand; ++i) {
            if (cand % primes[i] == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) {
            if (prime_cnt < PRIME_CAP) {
                primes[prime_cnt++] = cand;
            } else {
                /* Should never be reached for the intended limits */
                break;
            }
        }
        cand += 2;
    }
}

/* external function required by the build */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    size_t m = count_distinct_prime_factors((uint64_t)n);
    if (m == 0)
        return (seq_t)1;  /* should not happen, but safely handles n==1 */
    ensure_primes(m);
    return (seq_t)primes[m - 1];   /* primes[0] is the first prime (2) */
}
