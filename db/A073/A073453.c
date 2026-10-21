/*
 * A073453  Number of distinct remainders arising when n is divided by all
 *          primes up to n.
 * https://oeis.org/A073453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 337, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A073453: number of distinct remainders of n when divided by each prime ≤ n */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Compute a(n) for any n ≥ 1.  The implementation uses a simple
 * trial‑division sieve to generate primes ≤ n and a bitmap to keep
 * track of the distinct remainders.  This is O(n log log n) for the
 * prime generation and O(π(n)) for the remainder counting, which is
 * well below the 2‑second limit for n ≈ 10 000. */
seq_t a(idx_t n)
{
    if (n <= 1u) return 0;          /* a(1) = 0 by definition */

    /* visited[k] == 1 iff remainder k has already occurred */
    size_t mn = (size_t)n;
    uint8_t *visited = (uint8_t*)calloc(mn + 1, 1);
    if (!visited) return 0;          /* memory failure → 0 */

    /* dynamic array for the primes found so far */
    size_t prime_cap = 64;
    uint32_t *primes = (uint32_t*)malloc(prime_cap * sizeof(uint32_t));
    if (!primes) { free(visited); return 0; }
    size_t prime_cnt = 0;

    seq_t distinct = 0;

    for (idx_t p = 2; p <= n; ++p) {
        /* primality test using the primes already found */
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            idx_t q = primes[i];
            if (q * q > p) break;
            if (p % q == 0) { is_prime = false; break; }
        }

        if (is_prime) {
            /* store the new prime */
            if (prime_cnt == prime_cap) {
                prime_cap <<= 1;
                uint32_t *tmp = (uint32_t*)realloc(primes, prime_cap * sizeof(uint32_t));
                if (!tmp) { free(primes); free(visited); return distinct; }
                primes = tmp;
            }
            primes[prime_cnt++] = (uint32_t)p;

            /* compute remainder n mod p */
            seq_t r = (seq_t)n % (seq_t)p;
            size_t idx = (size_t)r;        /* idx < p ≤ n ≤ UINT64_MAX < SIZE_MAX here */
            if (!visited[idx]) {
                visited[idx] = 1;
                distinct = distinct + 1;
            }
        }
    }

    free(primes);
    free(visited);
    return distinct;
}
