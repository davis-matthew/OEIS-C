/*
 * A153081  Nonnegative numbers k such that 2k + 13 is prime.
 * https://oeis.org/A153081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes a(n) = (p - 13)/2 where p is the n‑th prime
   ≥ 13. It does so by building a small sieve up to a fixed limit
   once, then indexing into the list of primes.  The sieve is performed
   lazily on the first call to a(). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Constants for the sieve.  The limit is comfortably above the
   10 000th prime (≈ 104 729) so the sequence is defined for all
   required arguments. */
static const uint32_t LIMIT = 200000;          /* sieve limit */
static const size_t   MAX_PRIMES = 20000;      /* maximum primes to store */

/* Storage for the primes.  The array is allocated once on first use. */
static uint32_t *primes      = NULL;   /* array of primes */
static size_t   primes_cnt   = 0;      /* number of primes stored */
static size_t   primes_off   = 0;      /* index of first prime ≥ 13 */
static bool     primes_ready = false;  /* true when primes[] is populated */

/* Build the list of primes up to LIMIT using a simple Eratosthenes sieve. */
static void
ensure_primes(void)
{
    if (primes_ready)                     /* already built */
        return;

    /* Allocate storage for primes.  MAX_PRIMES is large enough for LIMIT. */
    primes = (uint32_t *)malloc(MAX_PRIMES * sizeof(uint32_t));
    if (!primes) {
        /* In a production build we would handle the error; here we
           simply return so the function will later return 0 for all n. */
        primes_ready = true;
        return;
    }

    /* Sieve of Eratosthenes.  is_composite[i] == 0  → i is prime. */
    char *is_composite = (char *)calloc(LIMIT + 1, 1);
    if (!is_composite) {
        /* Allocation failure – give up. */
        free(primes);
        primes = NULL;
        primes_ready = true;
        return;
    }

    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (!is_composite[i]) {
            if (primes_cnt < MAX_PRIMES)
                primes[primes_cnt++] = i;
            else
                break;   /* array full – should not happen for LIMIT=200k */

            if ((uint64_t)i * i <= LIMIT) {          /* mark composites */
                for (uint32_t j = i * i; j <= LIMIT; j += i)
                    is_composite[j] = 1;
            }
        }
    }

    free(is_composite);

    /* Find the first prime ≥ 13. */
    primes_off = 0;
    while (primes_off < primes_cnt && primes[primes_off] < 13)
        ++primes_off;

    primes_ready = true;
}

/* ------------------------------------------------------------------ */
/*  a(n) – value of the sequence A153081                                 */
/* ------------------------------------------------------------------ */
seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined for n < 1, but safe guard */

    ensure_primes();

    /* Compute 0‑based index of the desired prime. */
    size_t idx = primes_off + (size_t)(n - 1);

    if (idx >= primes_cnt || !primes) /* safety net for allocation failure */
        return 0;                     /* sentinel – sequence undefined for this n */

    uint32_t p = primes[idx];
    return ((seq_t)p - (seq_t)13) / 2;
}
