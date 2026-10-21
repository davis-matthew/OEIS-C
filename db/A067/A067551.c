/*
 * A067551  Determinant of an n X n matrix whose diagonal are the first n
 *          Primes of alternating sign and all other elements are 1's.
 * https://oeis.org/A067551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 9, i64 n <= 15, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* --------------------------------------------------------------------
   Formula: a(n) = ∏_{i=1..n} d_i  +  Σ_{j=1..n} ∏_{i≠j} d_i
   where d_i = (i odd ? p_i-1 : -(p_i+1))  and  p_i is the i-th prime.
   -------------------------------------------------------------------- */

static bool primes_ready = false;

/* Storage for the first 20000 primes (more than the 10000‑th prime). */
static uint32_t *prime_list = NULL;   /* array of primes */
static size_t   prime_count = 0;      /* number of primes stored */

/* Generate primes up to a limit big enough for the first 10000 primes. */
static void init_primes(void)
{
    const size_t LIMIT = 200000;     /* > 10000th prime = 104729          */
    uint8_t *sieve = (uint8_t *)calloc(LIMIT + 1, sizeof(uint8_t));
    if (!sieve) exit(1);            /* OOM, abort */

    prime_list = (uint32_t *)malloc(LIMIT * sizeof(uint32_t));
    if (!prime_list) exit(1);

    prime_count = 0;
    for (size_t i = 2; i <= LIMIT; ++i) {
        if (!sieve[i]) {
            prime_list[prime_count++] = (uint32_t)i;
            if (i * i <= LIMIT) {
                for (size_t j = i * i; j <= LIMIT; j += i)
                    sieve[j] = 1;
            }
        }
    }
    free(sieve);
    primes_ready = true;
}

/* --------------------------------------------------------------------
   Main sequence function
   -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!primes_ready) init_primes();

    if (n == 0) return 0;      /* undefined for n = 0, return 0 */

    /* Safety: n must not exceed number of pre‑generated primes. */
    if (n > prime_count) exit(1);

    seq_t prod = 1;    /* product of all d_i   */
    for (idx_t i = 0; i < n; ++i) {
        seq_t p = (seq_t)prime_list[i];      /* i starts at 0 -> p_(i+1) */
        seq_t d = ((i & 1) == 0) ? (p - 1)   /* i even -> odd index (1‑based)   */
                                 : -(p + 1);
        prod *= d;
    }

    /* sum of prod / d_j over all j */
    seq_t sum_term = 0;
    for (idx_t j = 0; j < n; ++j) {
        seq_t p = (seq_t)prime_list[j];
        seq_t d = ((j & 1) == 0) ? (p - 1) : -(p + 1);
        sum_term += prod / d;   /* division is exact */
    }

    return prod + sum_term;
}
