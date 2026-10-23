/*
 * A172071  Primes p such that either p - 5/2 -+ 7/2 is prime.
 * https://oeis.org/A172071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A172071: primes p such that p - 5/2 -+ 7/2 is prime.
   Equivalently, primes p where (p == 2) or (p > 2 and p-6 is prime).
*/
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Global storage for the pre‑computed sequence. */
static seq_t *g_seq   = NULL;   /* array of sequence terms          */
static size_t g_len   = 0;       /* number of terms stored          */

/* Build the sequence once, up to a limit that covers more than 10 000 terms. */
static void build_sequence(void)
{
    /* A sieve up to 20 000 000 is enough for the first 10 000 terms. */
    const idx_t LIMIT = 20000000U;

    /* Allocate the sieve.  Use explicit cast for C++ compatibility. */
    char *sieve = (char *)malloc((size_t)(LIMIT + 1U) * sizeof(char));
    if (!sieve) abort();

    /* Initialise all entries to “prime” except 0 and 1. */
    for (idx_t i = 2U; i <= LIMIT; ++i)
        sieve[i] = 1;

    /* Classic Eratosthenes sieve. */
    for (idx_t p = 2U; p * p <= LIMIT; ++p) {
        if (sieve[p]) {
            for (idx_t m = p * p; m <= LIMIT; m += p)
                sieve[m] = 0;
        }
    }

    /* Allocate space for the sequence. */
    size_t cap  = 20000U;                 /* initial capacity, more than enough */
    seq_t *seq  = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!seq) abort();

    size_t len = 0U;

    /* Include 2 explicitly. */
    if (sieve[2U]) {
        seq[len++] = (seq_t)2U;
    }

    /* Scan all primes p > 2 and collect those with p-6 prime. */
    for (idx_t p = 3U; p <= LIMIT; ++p) {
        if (!sieve[p]) continue;                /* p not prime */
        if (p >= 6U && sieve[p - 6U]) {          /* p-6 is prime */
            if (len == cap) {                   /* grow the array */
                cap *= 2;
                seq = (seq_t *)realloc(seq, cap * sizeof(seq_t));
                if (!seq) abort();
            }
            seq[len++] = (seq_t)p;
        }
    }

    free(sieve);          /* sieve no longer needed */

    /* Store the sequence in the global variables. */
    g_seq = seq;
    g_len = len;
}

/* External interface required by the framework. */
seq_t a(idx_t n)
{
    static bool ready = false;
    if (!ready) {
        build_sequence();
        ready = true;
    }

    if (n == 0U || n > g_len)
        return 0;           /* out of bounds – caller should not ask */

    return g_seq[n - 1U];
}
