/*
 * A145025  Numbers which are the average of two consecutive odd primes
 *          (A024675) together with primes which are the average of the
 *          previous prime and the following prime (A006562).
 * https://oeis.org/A145025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A145025: numbers n such that prevprime(n)+nextprime(n)=2n.
   The sequence consists of:
     1) the averages of every pair of consecutive odd primes,
     2) primes that are the average of their immediate prime neighbours.
   This implementation precomputes all required primes up to a fixed
   limit, gathers the two families of values, sorts them, and returns
   the n‑th element (offset 1).  The chosen limit guarantees at least
   10000 terms. */

/* Standard headers (C11 and C++17 compatible) */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* --- Configuration -------------------------------------------------------- */
#define PRIME_LIMIT 400000   /* Prime generation limit – enough for 10⁴ terms */

/* --- Global storage ------------------------------------------------------- */
static seq_t  *seq_cache = NULL;   /* Sorted sequence values             */
static size_t  seq_len   = 0;      /* Number of stored values             */
static bool    ready     = false;  /* Sequence generated?                  */

static uint32_t *primes     = NULL;  /* Array of primes up to PRIME_LIMIT   */
static size_t    prime_cnt  = 0;     /* Number of primes found              */

/* --- Helper: compare two seq_t for qsort --------------------------------- */
static int seq_compare(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* --- Prime sieve ---------------------------------------------------------- */
static void build_primes(void)
{
    /* Allocate storage for primes */
    primes = (uint32_t *)malloc((PRIME_LIMIT + 1) * sizeof(uint32_t));
    if (!primes) abort();

    bool *sieve = (bool *)calloc(PRIME_LIMIT + 1, sizeof(bool));
    if (!sieve) abort();

    /* Sieve of Eratosthenes (only composites are marked true) */
    for (uint32_t i = 2; (uint64_t)i * i <= PRIME_LIMIT; ++i) {
        if (!sieve[i]) {
            for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                sieve[j] = true;
        }
    }

    /* Store all primes */
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!sieve[i]) primes[prime_cnt++] = i;
    }

    free(sieve);
}

/* --- Sequence generation -------------------------------------------------- */
static void generate_sequence(void)
{
    if (ready) return;

    build_primes();

    /* Rough estimate of needed storage */
    size_t est = prime_cnt * 2 + 10;

    seq_cache = (seq_t *)malloc(est * sizeof(seq_t));
    if (!seq_cache) abort();

    size_t idx = 0;

    /* 1) Averages of consecutive odd primes (skip prime 2) */
    for (size_t i = 1; i + 1 < prime_cnt; ++i) {   /* i = 1 -> prime 3 */
        seq_t avg = ((seq_t)primes[i] + (seq_t)primes[i + 1]) / (seq_t)2;
        seq_cache[idx++] = avg;
    }

    /* 2) Primes that are the average of their immediate neighbours */
    for (size_t i = 1; i + 1 < prime_cnt; ++i) {   /* i = 1 -> prime 3 */
        seq_t left  = (seq_t)primes[i - 1];
        seq_t right = (seq_t)primes[i + 1];
        seq_t mid   = (seq_t)primes[i];
        if (left + right == (seq_t)2 * mid)
            seq_cache[idx++] = mid;
    }

    /* Resize array to actual size and sort */
    seq_cache = (seq_t *)realloc(seq_cache, idx * sizeof(seq_t));
    seq_len   = idx;
    ready     = true;

    qsort(seq_cache, seq_len, sizeof(seq_t), seq_compare);
}

/* --- Public interface ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* offset is 1; n=0 is undefined */
    generate_sequence();
    if ((idx_t)n > seq_len) abort(); /* should not happen for tested limits */
    return seq_cache[n - 1];
}
