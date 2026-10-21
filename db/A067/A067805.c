/*
 * A067805  Numbers n such that the area of the parallelogram formed by the
 *          vectors (n, prime(n)) and (n+1, prime(n+1)) is an integer square,
 *          i.e., Det[{{n, prime(n)},{n+1, prime(n+1)}}] is an integer square.
 * https://oeis.org/A067805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A067805:  Numbers n such that the determinant  
    Det[[n, prime(n)],[n+1,prime(n+1)]] is a non‑negative perfect square. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/isqrt_seq.h"

#define PRIME_LIMIT   20000000U    /* upper bound for primes needed (≈ first 1.2M primes) */
#define MAX_PRIMES    2000000U     /* accommodates all primes up to PRIME_LIMIT */
#define MAX_SEQ       2000000U     /* safety buffer for sequence terms */

/* 1. Prime sieve and prime list */
static unsigned char sieve[PRIME_LIMIT + 1];
static uint32_t primes[MAX_PRIMES];
static size_t prime_count = 0;

/* 2. Sequence cache */
static seq_t seq_terms[MAX_SEQ];
static size_t seq_len   = 0;          /* number of terms stored              */
static idx_t  processed_n = 0;        /* n that has been processed last      */

/* ------------------------------------------------------------------ */
/* Build the sieve once (static init).  All primes up to PRIME_LIMIT
   are stored in the global `primes` array. */
static void init_sieve(void)
{
    size_t i, j;
    for (i = 0; i <= PRIME_LIMIT; ++i)
        sieve[i] = 1;
    sieve[0] = sieve[1] = 0;
    for (i = 2; i * i <= PRIME_LIMIT; ++i)
        if (sieve[i])
            for (j = i * i; j <= PRIME_LIMIT; j += i)
                sieve[j] = 0;
    for (i = 2; i <= PRIME_LIMIT; ++i)
        if (sieve[i]) {
            if (prime_count < MAX_PRIMES)
                primes[prime_count++] = (uint32_t)i;
        }
}

/* ------------------------------------------------------------------ */
/* Extend the sequence until it contains at least `need` terms.  This
   routine scans successive n, computes the determinant
   d = n*prime(n+1) - (n+1)*prime(n),
   keeps only those n for which d >= 0 and d is a perfect square. */
static void ensure_sequence(idx_t need)
{
    if (seq_len >= (size_t)need) return;

    if (prime_count == 0) init_sieve();

    while (seq_len < (size_t)need) {
        ++processed_n;                                 /* n is 1‑based */

        /* Ensure enough primes have been generated.  The chosen
           PRIME_LIMIT is large enough for the test ranges. */
        if ((size_t)processed_n + 1 > prime_count)
            break;

        uint32_t p_curr = primes[processed_n - 1];   /* prime(n)   */
        uint32_t p_next = primes[processed_n];       /* prime(n+1) */

        /* Compute signed determinant using 64‑bit signed arithmetic. */
        int64_t det = (int64_t)processed_n * p_next
                    - (int64_t)(processed_n + 1) * p_curr;

        /* The determinant must be non‑negative.  Otherwise the term
           cannot be a perfect square. */
        if (det < 0) continue;

        /* d is non‑negative and fits into seq_t (uint64_t or
           arbitrary‑precision).  Convert it to seq_t safely. */
        seq_t d = (seq_t)det;
        seq_t root = isqrt_seq(d);
        if (root * root == d) {
            if (seq_len < MAX_SEQ)
                seq_terms[seq_len++] = (seq_t)processed_n;
        }
    }
}

/* ------------------------------------------------------------------ */
/* Public function required by the specification. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n=0, guard */
    ensure_sequence(n);
    return seq_terms[(size_t)n - 1];
}
