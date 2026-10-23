/*
 * A120497  Positive integers whose number of divisors is a perfect power.
 * https://oeis.org/A120497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Positive integers whose number of divisors is a perfect power
   OEIS A120497. The function a(idx_t n) returns the nth term
   (a(1) is the first term).  The implementation precomputes divisor
   counts by means of a linear sieve and stores all qualifying numbers
   in ascending order.  If the requested index is beyond the current
   precomputed range, the range is doubled until enough terms are
   available.  The algorithm runs in O(L) time where L is the current
   upper bound of precomputation. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Forward declaration of the required function. */
seq_t a(idx_t n);

/* ------------------------------------------------------------------ */
/* Helper: check whether an unsigned 32‑bit integer is a perfect power
 * (i.e. equal to b^e with e >= 2, or 1).                               */
static bool is_perfect_power(uint32_t x)
{
    if (x == 0) return false;
    if (x == 1) return true;
    for (uint32_t b = 2; (uint64_t)b * b <= x; ++b) {
        uint64_t val = (uint64_t)b * b;
        while (val < x) {
            if (val > UINT64_MAX / b) break; /* avoid overflow */
            val *= b;
        }
        if (val == x) return true;
    }
    return false;
}

/* ------------------------------------------------------------------ */
/* Sieve data structures.  These are reallocated whenever the
 * precomputed limit is increased.                                        */
static uint64_t *lp = NULL;          /* lowest prime factor */
static uint8_t  *cnt = NULL;         /* exponent of lp in the factorisation */
static uint32_t *divcnt = NULL;      /* divisor count d(n) */

/* Sequence storage.  seq[0] holds a(1), seq[1] holds a(2), …          */
static seq_t  *seq = NULL;
static size_t  seq_len = 0;          /* current number of stored terms */

/* Current precomputation limit.  All integers up to this limit are
   considered when building the sequence.                              */
static uint64_t precomp_limit = 0;
static bool initialized = false;

/* ------------------------------------------------------------------ */
/* Recalculate divisor counts up to 'limit' and rebuild the sequence. */
static void precompute(uint64_t limit)
{
    /* free old data */
    free(lp);   free(cnt);   free(divcnt);
    free(seq);

    /* allocate new data */
    lp = (uint64_t*)malloc((limit + 1) * sizeof(uint64_t));
    cnt = (uint8_t*)malloc((limit + 1) * sizeof(uint8_t));
    divcnt = (uint32_t*)malloc((limit + 1) * sizeof(uint32_t));

    uint64_t *primes = (uint64_t*)malloc((limit + 1) * sizeof(uint64_t));
    size_t primes_len = 0;
    if (!lp || !cnt || !divcnt || !primes) {
        /* memory allocation failure – abort */
        exit(EXIT_FAILURE);
    }

    /* initial conditions */
    lp[0] = 0; lp[1] = 0;
    cnt[0] = cnt[1] = 0;
    divcnt[0] = divcnt[1] = 1;

    /* linear sieve computing divisor count */
    for (uint64_t i = 2; i <= limit; ++i) {
        if (!lp[i]) {                  /* i is prime */
            lp[i] = i;
            primes[primes_len++] = i;
            cnt[i] = 1;
            divcnt[i] = 2;             /* prime has 2 divisors */
        }
        for (size_t j = 0; j < primes_len &&
                           primes[j] <= lp[i] &&
                           i * primes[j] <= limit; ++j) {
            uint64_t v = i * primes[j];
            lp[v] = primes[j];
            if (primes[j] == lp[i]) {
                cnt[v] = cnt[i] + 1;
                divcnt[v] = divcnt[i] / (cnt[i] + 1) * (cnt[v] + 1);
            } else {
                cnt[v] = 1;
                divcnt[v] = divcnt[i] * 2;
            }
        }
    }

    /* Build the sequence list */
    seq = (seq_t*)malloc((limit + 1) * sizeof(seq_t));
    if (!seq) exit(EXIT_FAILURE);
    seq_len = 0;
    for (uint64_t i = 1; i <= limit; ++i) {
        if (is_perfect_power(divcnt[i])) {
            seq[seq_len++] = (seq_t)i;
        }
    }

    free(primes);
    precomp_limit = limit;
}

/* ------------------------------------------------------------------ */
/* Ensure that at least n terms have been precomputed. */
static void ensure_precomputed(idx_t n)
{
    if (!initialized) {
        precompute(200000);          /* initial guess */
        initialized = true;
    }
    while (seq_len < (size_t)n) {
        precompute(precomp_limit * 2); /* double the limit if needed */
    }
}

/* ------------------------------------------------------------------ */
/* Public function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for 0, but safe */
    ensure_precomputed(n);
    return seq[n - 1];
}
