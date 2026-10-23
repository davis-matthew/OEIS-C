/*
 * A165914  a(n) = sum of the squarefree quadratic residues of n.
 * https://oeis.org/A165914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the squarefree quadratic residues of n. * a(1) = 0
 *            * a(2) = 1 * . *
 * Range:     u8 n <= 42, u16 n <= 672, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Utility: generate primes up to LIMIT once. */
static uint64_t *prime_list = NULL;
static size_t prime_cnt = 0;
static bool primes_ready = false;

static void init_primes(uint64_t limit)
{
    if (prime_cnt) return;              /* already initialized */

    /* simple sieve */
    uint8_t *sieve = (uint8_t *)calloc(limit + 1, sizeof(uint8_t));
    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (!sieve[i]) {
            for (uint64_t j = i * i; j <= limit; j += i)
                sieve[j] = 1;
        }
    }

    /* count primes */
    prime_cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (!sieve[i]) ++prime_cnt;

    /* store primes */
    prime_list = (uint64_t *)malloc(prime_cnt * sizeof(uint64_t));
    size_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (!sieve[i]) prime_list[idx++] = i;

    free(sieve);
    primes_ready = true;
}

/* check if an unsigned integer v is squarefree */
static bool is_squarefree_uint(uint64_t v)
{
    if (v == 0) return false;
    /* test primal squares up to sqrt(v) */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        uint64_t sq = p * p;
        if (sq > v) break;
        if (v % sq == 0) return false;
    }
    return true;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* lazy prime table: 10⁶ is more than enough for n up to ~10⁴ */
    if (!primes_ready) init_primes(100000);

    if (n == 0) return 0;          /* safety, although not needed (n≥1) */

    seq_t sum = 0;

    bool *seen = (bool *)calloc((size_t)n, sizeof(bool));
    if (!seen) return 0;           /* allocation failure: return 0 */

    for (idx_t i = 0; i < n; ++i) {
        idx_t r = (i * i) % n;
        if (!seen[r]) {
            seen[r] = true;
            if (is_squarefree_uint(r))
                sum += (seq_t)r;
        }
    }

    free(seen);
    return sum;
}
