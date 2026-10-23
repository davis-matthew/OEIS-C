/*
 * A152468  Smallest of five consecutive primes whose sum is a prime.
 * https://oeis.org/A152468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 344, u32 n <= 345, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A152468: smallest of five consecutive primes whose sum is a prime.
   The function builds the sequence on demand using a sliding window over an
   incrementally generated list of primes.  */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* External primality test for big integers. */
#include "../utils/is_prime_big.h"

/* -------------------------------------------------------------------- */
/* Prime generation utilities                                         */

static uint64_t *primes      = NULL; /* storage for generated primes          */
static size_t   primes_len  = 0;     /* number of primes stored              */
static size_t   primes_cap  = 0;     /* allocated capacity of primes array   */
static uint64_t next_candidate = 2;  /* next integer to test for primality   */

/* Returns the next prime number and appends it to the primes array.     */
static uint64_t next_prime(void)
{
    while (true) {
        uint64_t cand = next_candidate;

        if (cand == 2) {                   /* handle the only even prime */
            next_candidate = 3;             /* afterwards test only odd numbers */
            return 2;
        }

        next_candidate += 2;                /* skip even numbers */

        bool is_prime = true;
        for (size_t i = 0; i < primes_len && primes[i] * primes[i] <= cand; ++i) {
            if (cand % primes[i] == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime)
            return cand;
    }
}

/* Ensure that at least (needed + 1) primes are present in the primes array. */
static void ensure_prime_at_least(size_t needed)
{
    while (primes_len <= needed) {
        /* Grow the primes array if necessary */
        if (primes_cap == primes_len) {
            size_t new_cap = primes_cap ? primes_cap * 2 : 16;
            uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
            primes = tmp;
            primes_cap = new_cap;
        }

        primes[primes_len++] = next_prime();
    }
}

/* -------------------------------------------------------------------- */
/* Sequence storage utilities                                        */

static seq_t *seq   = NULL; /* array holding a(1), a(2), ...              */
static size_t seq_len = 0;  /* number of computed sequence terms           */
static size_t seq_cap = 0;  /* allocated capacity of seq array            */

static void seq_append(seq_t val)
{
    if (seq_cap == seq_len) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq = tmp;
        seq_cap = new_cap;
    }
    seq[seq_len++] = val;
}

/* -------------------------------------------------------------------- */
/* Main sequence function                                            */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For n == 0 we return 0. */
    if (n == 0)
        return 0;

    /* Index of the first prime in the current sliding window. */
    static idx_t window_start = 0;

    /* Generate terms up to the requested index. */
    while (seq_len < n) {
        /* Ensure we have enough primes for the current window. */
        ensure_prime_at_least((size_t)window_start + 4);

        /* Compute the sum of the five consecutive primes. */
        seq_t sum =
            (seq_t)primes[window_start]     +
            (seq_t)primes[window_start + 1] +
            (seq_t)primes[window_start + 2] +
            (seq_t)primes[window_start + 3] +
            (seq_t)primes[window_start + 4];

        /* If the sum is prime, record the smallest prime of the window. */
        if (is_prime_big(sum))
            seq_append((seq_t)primes[window_start]);

        ++window_start;
    }

    return seq[n - 1];
}
