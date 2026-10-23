/*
 * A025117  a(n) = s(1)t(n) + s(2)t(n-1) + ... + s(k)t(n-k+1), where k = [ n/2
 *          ], s = (odd natural numbers), t = (primes).
 * https://oeis.org/A025117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1..⌊(n+1)/2⌋} (2*i-1) * prime(n+2-i)
 * Range:     u8 n <= 7, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function implements A025117 directly using the definition. */

#include <stdlib.h>
#include <stdbool.h>

/* Cached list of primes (0‑based index: primes[0] == 2). */
static seq_t *prime_cache = NULL;
static size_t prime_cnt = 0;
static size_t prime_cap = 0;
static idx_t next_candidate = 3;

/* Ensure that prime_cache contains at least 'needed' primes. */
static void ensure_primes(idx_t needed)
{
    if (prime_cnt >= needed)
        return;

    /* First prime is 2. Allocate space if necessary. */
    if (prime_cnt == 0) {
        prime_cap = 16;
        prime_cache = (seq_t *)malloc(prime_cap * sizeof(seq_t));
        prime_cache[0] = (seq_t)2;
        prime_cnt = 1;
        next_candidate = 3;
    }

    /* Generate primes until we have enough. */
    while (prime_cnt < needed) {
        idx_t cand = next_candidate;
        next_candidate++;

        seq_t cand_seq = (seq_t)cand;
        bool is_prime = true;

        for (size_t j = 0; j < prime_cnt; ++j) {
            seq_t sq = prime_cache[j] * prime_cache[j];
            if (sq > cand_seq)
                break;
            if (cand_seq % prime_cache[j] == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            if (prime_cnt >= prime_cap) {
                prime_cap *= 2;
                prime_cache = (seq_t *)realloc(prime_cache, prime_cap * sizeof(seq_t));
            }
            prime_cache[prime_cnt++] = cand_seq;
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                        /* offset is 1; undefined for n==0 */

    /* We need primes up to index n+1 (1‑based). */
    ensure_primes(n + 1);

    seq_t sum = 0;
    idx_t k = (n + 1) / 2;                /* floor((n+1)/2) */

    for (idx_t i = 1; i <= k; ++i) {
        seq_t s = (seq_t)(2 * i - 1);
        idx_t j = n + 2 - i;              /* prime index (1‑based) */
        seq_t t = prime_cache[j - 1];     /* array is 0‑based */
        sum += s * t;
    }
    return sum;
}
