/*
 * A056172  Number of non-unitary prime divisors of n!.
 * https://oeis.org/A056172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = π(n/2) – the number of primes ≤ ⌊n/2⌋
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of non‑unitary prime divisors of n! */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

static bool *sieve = NULL;          /* primality flags */
static uint64_t sieve_max = 0;      /* largest number for which sieve is built */

static void build_sieve(uint64_t limit)
{
    if (limit <= sieve_max) return;

    /* Allocate or reallocate the sieve array */
    bool *new_sieve = (bool *)realloc(sieve, (limit + 1) * sizeof(bool));
    if (!new_sieve) return; /* allocation failed – leave sieve empty */
    sieve = new_sieve;

    /* Initialize all positions as true; set 0 and 1 to false */
    for (uint64_t i = 0; i <= limit; ++i) sieve[i] = true;
    if (limit >= 0) sieve[0] = false;
    if (limit >= 1) sieve[1] = false;

    /* Eratosthenes sieve */
    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (sieve[i]) {
            for (uint64_t j = i * i; j <= limit; j += i) {
                sieve[j] = false;
            }
        }
    }
    sieve_max = limit;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* offset starts at 1 (for completeness) */
    uint64_t half = n / 2;             /* floor(n/2) */

    build_sieve(half);

    seq_t cnt = 0;
    for (uint64_t i = 2; i <= half; ++i) {
        if (sieve[i]) ++cnt;           /* count primes ≤ half */
    }
    return cnt;
}
