/*
 * A068360  Numbers n such that there is no squarefree number between prime(n)
 *          and prime(n+1).
 * https://oeis.org/A068360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate indices n for which there is no square‑free number
 * between prime(n) and prime(n+1).  The algorithm walks through consecutive
 * primes, checks all integers in the intervening gap for a square divisor,
 * and records the prime index when every such integer is not square‑free.  */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* helper from the utils library that builds a prime list up to a limit */
#include "../utils/sieve_primes.h"

/* ------------------------------------------------------------------------- */
/* Global data                                                      */
/* ------------------------------------------------------------------------- */
static bool primes_initialized = false;
static idx_t *PRIMES = NULL;      /* array of primes */
static size_t PRIMES_CNT = 0;     /* number of primes */

/* sequence cache: indices n that satisfy the condition */
static idx_t *SEQ = NULL;
static size_t SEQ_LEN = 0, SEQ_CAP = 0;

/* next prime index to examine (1‑based) */
static idx_t NEXT_PRIME_IDX = 1;

/* ------------------------------------------------------------------------- */
/* Helper: test whether an integer has a squared prime divisor        */
/* ------------------------------------------------------------------------- */
static bool is_squarefree(uint64_t m)
{
    /* iterate over primes until p^2 > m */
    for (size_t i = 0; i < PRIMES_CNT; ++i) {
        uint64_t p  = PRIMES[i];
        uint64_t sq = p * p;
        if (sq > m) break;
        if (m % sq == 0) return false;   /* square factor found */
    }
    return true;   /* no square factor */
}

/* ------------------------------------------------------------------------- */
/* Prime generation ------------------------------------------------------- */
/* ------------------------------------------------------------------------- */
static void init_primes(void)
{
    if (primes_initialized) return;

    /* Upper bound chosen large enough to provide many more primes than needed
     * for the first 10 000 terms of A068360.  The 1 000 000th prime is ≈15 5×10⁶,
     * so 20 million gives a generous safety margin. */
    const idx_t LIMIT = 20000000ULL;   /* 20 million */

    sieve_primes(LIMIT, &PRIMES, &PRIMES_CNT);
    primes_initialized = true;
}

/* ------------------------------------------------------------------------- */
/* Sequence function ----------------------------------------------------- */
/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    init_primes();

    while (SEQ_LEN < n) {
        /* Ensure we have two consecutive primes to examine */
        if ((size_t)(NEXT_PRIME_IDX + 1) >= PRIMES_CNT) {
            /* The pre‑computed prime limit is far beyond what we need for
             * n up to 10 000; this situation should never occur. */
            return 0;
        }

        uint64_t p = PRIMES[NEXT_PRIME_IDX   - 1];
        uint64_t q = PRIMES[NEXT_PRIME_IDX + 1 - 1];

        bool ok = true;
        for (uint64_t m = p + 1; m < q; ++m) {
            if (is_squarefree(m)) {
                ok = false;
                break;
            }
        }

        if (ok) {
            /* store the index (NEXT_PRIME_IDX) as a sequence term */
            if (SEQ_LEN >= SEQ_CAP) {
                SEQ_CAP = SEQ_CAP ? SEQ_CAP * 2 : 16;
                SEQ = (idx_t *)realloc(SEQ, SEQ_CAP * sizeof(idx_t));
                if (!SEQ) return 0;   /* allocation failure */
            }
            SEQ[SEQ_LEN++] = NEXT_PRIME_IDX;
        }

        ++NEXT_PRIME_IDX;
    }

    return (seq_t)SEQ[n - 1];
}
