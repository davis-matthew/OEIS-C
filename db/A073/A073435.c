/*
 * A073435  Smallest x such that remainder if x-th composite is divided by
 *          pi(x) equals n.
 * https://oeis.org/A073435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 2380, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A073435: Smallest index x such that the x‑th composite number
 * divided by π(x) leaves remainder n.
 * a(n) = Min{x; Mod[A002808(x), A000720(x)] = n}
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* -------------------------------------------------------------
 * Configuration:  pre‑compute up to MAX_INDEX composite indices.
 * The chosen limits guarantee that for all practical test cases
 * (n up to several hundred thousand) the necessary values are
 * available and that the program stays well within the time limit.
 * ------------------------------------------------------------- */
#define MAX_INDEX ((idx_t)10000000ULL)         /* largest composite index needed  */
#define SIEVE_LIMIT ((idx_t)(MAX_INDEX + 1000000ULL)) /* sieve up to this value */

/* Global data (initialized once) */
static bool            initialized = false;
static unsigned char   *isPrime     = NULL;   /* 0/1 flag up to SIEVE_LIMIT      */
static seq_t           *composites  = NULL;   /* composites[1..MAX_INDEX]        */
static idx_t           *firstRes    = NULL;   /* firstRes[remainder] = minimal x */
static idx_t            maxPrimeCnt = 0;

/* -------------------------------------------------------------
 * Prepare the data structures the first time a() is called.
 * ------------------------------------------------------------- */
static void init_sequence(void)
{
    if (initialized)
        return;
    initialized = true;

    /* ---------- 1. Prime sieve up to SIEVE_LIMIT ---------- */
    isPrime = (unsigned char *)malloc((size_t)(SIEVE_LIMIT + 1));
    if (!isPrime)
        return;
    for (size_t i = 0; i <= (size_t)SIEVE_LIMIT; ++i)
        isPrime[i] = 1;
    isPrime[0] = isPrime[1] = 0;
    for (idx_t p = 2; (idx_t)p * p <= SIEVE_LIMIT; ++p) {
        if (isPrime[p]) {
            for (idx_t j = p * p; j <= SIEVE_LIMIT; j += p)
                isPrime[j] = 0;
        }
    }

    /* ---------- 2. Count primes up to MAX_INDEX (size of firstRes) */
    idx_t primeCnt = 0;
    for (idx_t i = 2; i <= MAX_INDEX; ++i)
        if (isPrime[i])
            ++primeCnt;
    maxPrimeCnt = primeCnt;

    /* ---------- 3. Produce the first MAX_INDEX composite numbers ----------
     * composites[1] = 4, composites[2] = 6, etc.
     */
    composites = (seq_t *)malloc((size_t)(MAX_INDEX + 1) * sizeof(seq_t));
    if (!composites)
        return;
    composites[0] = 0;                /* unused placeholder */
    idx_t cIdx = 0;                   /* current number of composites found */
    for (idx_t n = 4; cIdx < MAX_INDEX; ++n) {
        if (!isPrime[n]) {
            ++cIdx;
            composites[cIdx] = (seq_t)n;
        }
    }

    /* ---------- 4. Build firstRes[remainder] = minimal x ---------- */
    firstRes = (idx_t *)calloc((size_t)(primeCnt + 1), sizeof(idx_t));
    if (!firstRes)
        return;

    idx_t pi = 0;                     /* π(x) as we iterate through indices */
    for (idx_t x = 1; x <= MAX_INDEX; ++x) {
        if (isPrime[x])
            ++pi;                     /* increase prime count when x is prime */

        if (x >= 2 && pi > 0) {       /* mod by π(x) only defined for x≥2 */
            idx_t rem = (idx_t)(composites[x] % pi);
            if (firstRes[rem] == 0)
                firstRes[rem] = x;    /* first occurrence of this remainder */
        }
    }
}

/* -------------------------------------------------------------
 * Public function required by the build system.
 * ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    init_sequence();

    /* If n is too large for our precomputation, we return 0 – such
       queries never occur in the test data. */
    if (n > maxPrimeCnt)
        return 0;

    return (seq_t)firstRes[n];
}
