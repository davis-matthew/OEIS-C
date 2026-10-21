/*
 * A099644  a[n]=Mod[q(n),PrimePi[q(n)]]=Mod[A001359(n), A000720(A001359(n))]
 *          where q(n) is the n-th lesser-twin-prime. A004648 restricted to
 *          lesser twins.
 * https://oeis.org/A099644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Mod[q(n), PrimePi[q(n)]] where q(n) is the nth lower twin
 *            prime (A001359) and PrimePi(x) is A000720(x)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define MAX_LIMIT 2000000ULL   /* sufficient for n ≤ 10 000 */

/* ---------- static storage ---------- */
static idx_t *prime_mark   = NULL;   /* 0 = prime, 1 = composite */
static idx_t *prime_pi     = NULL;   /* prefix prime counts */
static idx_t *twin_primes  = NULL;   /* lower members of twin primes */
static size_t twin_cnt     = 0;
static size_t twin_cap     = 0;
static bool initialized    = false;

/* ---------- helper to grow the twin list ---------- */
static void push_twin(idx_t p)
{
    if (twin_cnt == twin_cap) {
        twin_cap = twin_cap ? twin_cap * 2 : 32;
        twin_primes = (idx_t *)realloc(twin_primes, twin_cap * sizeof(idx_t));
    }
    twin_primes[twin_cnt++] = p;
}

/* ---------- initialization: sieve, build prime_pi and twin list ---------- */
static void init(void)
{
    /* sieve of Eratosthenes for primes up to MAX_LIMIT */
    prime_mark = (idx_t *)calloc(MAX_LIMIT + 1, sizeof(idx_t));
    for (idx_t i = 2; i * i <= MAX_LIMIT; ++i) {
        if (!prime_mark[i]) {
            for (idx_t j = i * i; j <= MAX_LIMIT; j += i)
                prime_mark[j] = 1;
        }
    }

    /* prefix count of primes */
    prime_pi = (idx_t *)malloc((MAX_LIMIT + 1) * sizeof(idx_t));
    idx_t pcnt = 0;
    for (idx_t i = 0; i <= MAX_LIMIT; ++i) {
        if (i >= 2 && !prime_mark[i])
            ++pcnt;
        prime_pi[i] = pcnt;
    }

    /* build list of lower twin primes */
    twin_primes = NULL;
    twin_cnt    = 0;
    twin_cap    = 0;
    for (idx_t p = 2; p + 2 <= MAX_LIMIT; ++p) {
        if (!prime_mark[p] && !prime_mark[p + 2])
            push_twin(p);
    }
}

/* ---------- sequence function ---------- */
seq_t a(idx_t n)
{
    /* lazy initialization */
    if (!initialized) {
        init();
        initialized = true;
    }

    if (n == 0)          /* outside OEIS offset; value undefined */
        return (seq_t)0;

    if (n > twin_cnt)    /* safety: limit too small for requested n */
        return (seq_t)0;

    idx_t q     = twin_primes[n - 1];
    idx_t piq   = prime_pi[q];
    seq_t result = (seq_t)(q % piq);
    return result;
}
