/*
 * A142026  Primes congruent to 22 mod 31.
 * https://oeis.org/A142026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 209, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth prime congruent to 22 mod 31 by sieving. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Storage for the primes in the AP 31k + 22. */
static uint64_t *ap_primes = NULL;   /* array of primes */
static size_t   ap_count  = 0;       /* number of primes stored */
static size_t   cur_limit = 0;       /* highest value currently sieved */

/* Ensure that at least n primes in the AP are available. */
static void
ensure_primes(idx_t n)
{
    /* n is at least 1, but check anyway */
    if (n == 0) return;

    size_t needed = (size_t)n;
    if (ap_count >= needed) return;        /* already sufficient */

    /* Rough upper bound for n‑th prime in this progression.
       Using n*400+1000 gives enough room for n up to many millions. */
    size_t new_limit = needed * 400 + 1000;
    if (new_limit <= cur_limit)
        new_limit = cur_limit + 4000;      /* increase a bit */

    /* Allocate a simple sieve array: 1 = probably prime, 0 = composite. */
    unsigned char *is_p = (unsigned char *)malloc(new_limit + 1);
    if (!is_p) abort();
    for (size_t i = 0; i <= new_limit; ++i) is_p[i] = 1;
    if (new_limit >= 0) is_p[0] = 0;
    if (new_limit >= 1) is_p[1] = 0;

    /* Eratosthenes sieve.  Use (uint64_t) for the square test to avoid overflow. */
    for (size_t p = 2; (uint64_t)p * p <= new_limit; ++p) {
        if (!is_p[p]) continue;
        for (size_t m = p * p; m <= new_limit; m += p)
            is_p[m] = 0;
    }

    /* Count primes congruent to 22 mod 31. */
    size_t count = 0;
    for (size_t x = 22; x <= new_limit; x += 31)
        if (is_p[x]) ++count;

    /* Allocate new array and fill it. */
    uint64_t *new_arr = NULL;
    if (count > 0) {
        new_arr = (uint64_t *)malloc(count * sizeof(uint64_t));
        if (!new_arr) abort();
        size_t idx = 0;
        for (size_t x = 22; x <= new_limit; x += 31)
            if (is_p[x]) new_arr[idx++] = (uint64_t)x;
    }

    /* Replace the old data. */
    free(ap_primes);
    ap_primes = new_arr;
    ap_count  = count;
    cur_limit = new_limit;

    free(is_p);
}

seq_t
a(idx_t n)
{
    if (n == 0)                          /* undefined for n < 1; return 0 */
        return (seq_t)0;

    ensure_primes(n);
    /* n is at least 1, and we have enough primes. */
    return (seq_t)ap_primes[(size_t)n - 1];
}
