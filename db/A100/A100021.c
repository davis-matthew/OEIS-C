/*
 * A100021  a(n) = 3*prime(n) - prime(n+1) - 3.
 * https://oeis.org/A100021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = 3*prime(n) - prime(n+1) - 3
 * Range:     u8 n <= 32, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A100021 using incremental prime generation. */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *primes = NULL;          /* Dynamic array of primes (1-indexed). */
static idx_t   primes_count = 0;      /* Number of primes stored. */
static idx_t   primes_capacity = 0;   /* Allocated capacity of primes array. */

/* Ensure that we have at least k primes in the array. */
static void
ensure_primes_up_to(idx_t k)
{
    if (primes_count >= k) return;

    /* Allocate or grow the prime array. */
    if (primes_capacity == 0) {
        primes_capacity = 128;
        primes = (seq_t *)malloc(primes_capacity * sizeof(seq_t));
    }
    while (primes_capacity < k) {
        primes_capacity <<= 1;
        primes = (seq_t *)realloc(primes, primes_capacity * sizeof(seq_t));
    }

    /* Seed the first prime if necessary. */
    if (primes_count == 0) {
        primes[0] = 2;
        primes_count = 1;
    }

    /* Incrementally generate primes until we have k of them. */
    while (primes_count < k) {
        seq_t cand = primes[primes_count - 1] + 1;   /* Next candidate. */
        if (cand <= 2) cand = 3;
        if ((cand % 2) == 0) cand += 1;              /* Ensure odd. */

        for (;;) {
            bool is_prime = true;
            seq_t sq = 0;
            for (idx_t j = 0; j < primes_count; ++j) {
                seq_t p = primes[j];
                sq = p * p;
                if (sq > cand) break;
                if ((cand % p) == 0) { is_prime = false; break; }
            }
            if (is_prime) {
                primes[primes_count++] = cand;
                break;          /* Found the next prime. */
            }
            cand += 2;          /* Skip even numbers. */
        }
    }
}

/* Return the nth term of A100021 (n >= 1). */
seq_t
a(idx_t n)
{
    if (n == 0) return 0;               /* Undefined for n = 0, safe guard. */

    ensure_primes_up_to(n + 1);          /* Need prime(n) and prime(n+1). */

    seq_t p_n   = primes[n - 1];         /* prime(n)   (1-indexed). */
    seq_t p_n1  = primes[n];             /* prime(n+1) */

    return 3 * p_n - p_n1 - 3;
}
