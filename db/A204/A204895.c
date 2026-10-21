/*
 * A204895  The prime q such that n divides p-q, where p>q is the least prime
 *          for which such a prime q exists.
 * https://oeis.org/A204895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the prime q such that n divides (p–q) where p>q is the
 * least prime for which such a prime q exists. The function searches primes
 * until it finds the first pair (p,q) with equal residue modulo n and 
 * returns q.
 */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

static uint64_t *prime_list = NULL;
static size_t prime_cap   = 0;
static size_t prime_cnt   = 0;

/* Integer square‑root of a 64‑bit number (floor). */
static inline uint64_t isqrt_u64(uint64_t n)
{
    uint64_t x   = 0;
    uint64_t bit = 1ULL << 62;
    while (bit > n) bit >>= 2;
    while (bit) {
        uint64_t z = x + bit;
        if (n >= z) {
            n -= z;
            x = (x >> 1) + bit;
        } else {
            x >>= 1;
        }
        bit >>= 2;
    }
    return x;
}

/* Append the next prime number to prime_list. */
static void add_next_prime(void)
{
    uint64_t cand = (prime_cnt == 0) ? 2 : prime_list[prime_cnt - 1] + 1;
    for (;; ++cand) {
        uint64_t limit = isqrt_u64(cand);
        int prime = 1;
        for (size_t i = 0; i < prime_cnt && prime_list[i] <= limit; ++i) {
            if (cand % prime_list[i] == 0) { prime = 0; break; }
        }
        if (prime) {
            if (prime_cnt == prime_cap) {
                prime_cap = (prime_cap == 0) ? 16 : prime_cap * 2;
                uint64_t *tmp = (uint64_t *)realloc(prime_list,
                                                    prime_cap * sizeof(uint64_t));
                if (!tmp) abort();          /* allocation failure */
                prime_list = tmp;
            }
            prime_list[prime_cnt++] = cand;
            return;
        }
    }
}

/* Compute a(n) as defined above.  n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* not used, but safe */

    size_t N = (size_t)n;
    seq_t *first = (seq_t *)calloc(N, sizeof(seq_t));
    if (!first) return 0;               /* memory failure, unlikely */

    size_t idx = 0;
    for (;;) {
        if (idx == prime_cnt) add_next_prime();
        uint64_t p = prime_list[idx++];
        uint64_t r = p % (uint64_t)n;   /* residue in 0..n-1 */
        size_t i = (size_t)r;
        if (first[i] != 0) {
            seq_t result = first[i];
            free(first);
            return result;
        }
        first[i] = (seq_t)p;
    }
}
