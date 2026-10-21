/*
 * A067677  Diagonals of the prime-composite array, B(m,n) which are zeros
 *          from the Second Borve Conjecture.
 * https://oeis.org/A067677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   ensure_mem_cap
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A067677 – Diagonals of the prime‑composite array that are
    zero‑only (Second Borve Conjecture).
    The value a(n) is the n‑th m for which, for every k ≥ 1,
    the k‑th prime p_k does not divide the (m+k‑1)‑st composite
    c_{m+k-1}.  The test stops successfully when
    c_{m+k-1} < p_k, because all larger primes then also do not
    divide the composite. */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ensure_mem_cap.h"

/* ------------------------------------------------------------------ */
/*  Constants and static data                                         */
/* ------------------------------------------------------------------ */
#define SIEVE_LIMIT 20000000   /* enough for all requested terms */

static int  *primes      = NULL;  /* 1‑based: primes[1] = 2   */
static int  *composites  = NULL;  /* 1‑based: composites[1] = 4*/
static size_t primes_cnt      = 0;
static size_t composites_cnt  = 0;
static size_t primes_cap      = 0;
static size_t composites_cap  = 0;

static idx_t *zeroDiagonals = NULL; /* 1‑based: zeroDiagonals[1] = first term */
static size_t zeroCnt  = 0;
static size_t zeroCap  = 0;
static idx_t nextCandidate = 1;

/* ------------------------------------------------------------------ */
/*  Helper routines                                                   */

static void build_sieve(void)
{
    static int built = 0;
    if (built) return;
    built = 1;

    char *isPrime = (char*)malloc(SIEVE_LIMIT + 1);
    if (!isPrime) exit(1);
    for (size_t i = 0; i <= SIEVE_LIMIT; ++i) isPrime[i] = 1;
    isPrime[0] = isPrime[1] = 0;
    for (size_t i = 2; i * i <= SIEVE_LIMIT; ++i) {
        if (isPrime[i]) {
            for (size_t j = i * i; j <= SIEVE_LIMIT; j += i)
                isPrime[j] = 0;
        }
    }
    primes_cnt = 0;
    composites_cnt = 0;
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (isPrime[i]) {
            ensure_mem_cap((void**)&primes, &primes_cap, primes_cnt + 1, sizeof(int), "primes");
            primes[++primes_cnt] = (int)i;
        } else if (i >= 4) {          /* composites start at 4 */
            ensure_mem_cap((void**)&composites, &composites_cap, composites_cnt + 1, sizeof(int), "composites");
            composites[++composites_cnt] = (int)i;
        }
    }
    free(isPrime);
}

/*  Returns 1 if the diagonal starting at m is zero‑only, 0 otherwise.   */
static int check_zero_diagonal(idx_t m)
{
    for (idx_t k = 1; ; ++k) {
        idx_t compIdx = m + k - 1;          /* composite index (1‑based) */
        if (compIdx > composites_cnt)      /* we don't have enough composites    */
            return 0;
        int compVal = composites[compIdx];
        if (k > primes_cnt) return 0;      /* no more primes available           */
        int prime = primes[k];
        if (compVal % prime == 0)          /* divisible → not zero‑only          */
            return 0;
        if ((idx_t)compVal < (idx_t)prime) /* composite smaller → all rest ok   */
            return 1;
        /* otherwise continue with next prime/composite pair      */
    }
}

/* ------------------------------------------------------------------ */
/*  Public sequence function                                           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* initialize once */
    static int init_done = 0;
    if (!init_done) {
        build_sieve();
        zeroCap = 256;
        zeroDiagonals = (idx_t*)malloc(zeroCap * sizeof(idx_t));
        if (!zeroDiagonals) exit(1);
        zeroDiagonals[0] = 0; /* unused sentinel */
        zeroCnt = 0;
        nextCandidate = 1;
        init_done = 1;
    }

    /* generate terms until we have the n‑th */
    while (zeroCnt <= n) {
        if (check_zero_diagonal(nextCandidate)) {
            ensure_mem_cap((void**)&zeroDiagonals, &zeroCap, zeroCnt + 2, sizeof(idx_t), "zeroDiagonals");
            zeroDiagonals[++zeroCnt] = nextCandidate;
        }
        ++nextCandidate;
    }
    return (seq_t)zeroDiagonals[n];
}
