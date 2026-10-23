/*
 * A019518  Smarandache-Wellin numbers: a(n) is the concatenation of first n
 *          primes (written in base 10).
 * https://oeis.org/A019518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, changed
 * Formula:   a(n) = concatenation of the first n primes in base 10
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 21,
 *            big any n
 * Helpers:   digit_len, pow10, sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Utility functions provided by the build environment */
#include "../utils/sieve_primes.h"   /* void sieve_primes(idx_t limit, idx_t **primes_out, size_t *cnt_out); */
#include "../utils/digit_len.h"      /* int digit_len(seq_t x); */
#include "../utils/pow10.h"          /* seq_t pow10(idx_t e); */

seq_t a(idx_t n)
{
    /* OEIS offset is 1; for n == 0 return 0 (unused) */
    if (n == 0) return 0;

    /* Rough upper bound for the n‑th prime: p_n < 20*n for n ≥ 6
       (the 10 000‑th prime is 104 729 < 200 000). */
    idx_t limit = n < 6 ? (idx_t)15 : (idx_t)(n * 20 + 100);

    idx_t *primes = NULL;           /* array of primes returned by sieve */
    size_t prime_cnt = 0;           /* number of primes found */

    /* Generate primes up to 'limit' until we have at least n of them. */
    while (true) {
        if (primes) free(primes);   /* free any previous allocation */
        sieve_primes(limit, &primes, &prime_cnt);
        if (prime_cnt >= (size_t)n) break;
        /* Not enough primes – increase limit and try again. */
        limit *= 2;
    }

    /* Concatenate the first n primes. */
    seq_t result = 0;
    for (idx_t i = 0; i < n; ++i) {
        seq_t p   = (seq_t)primes[i];
        int   d   = digit_len(p);   /* number of decimal digits of p */
        result = result * pow10((idx_t)d) + p;
    }

    free(primes);                   /* clean up */
    return result;
}
