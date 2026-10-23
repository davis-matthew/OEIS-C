/*
 * A164653  a(1) = 1, for n>=2: a(n) = sum of two consecutive noncomposite
 *          numbers A008578.
 * https://oeis.org/A164653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for n = 1 a(n) = A008578(n-1)+A008578(n) for n >= 2
 *            (A008578 is the sequence of non‑composite numbers:
 *            1,2,3,5,7,11,13,…)
 * Range:     u8 n <= 32, u16 n <= 3514, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* ----- Prime generator --------------------------------------------------- */
/*  We lazily generate enough primes to answer any call to a(n).
    primes[0] = 2, primes[1] = 3, primes[2] = 5, …                   */
static uint64_t *primes = NULL;     /* array of primes                     */
static size_t   prime_cnt = 0;      /* number of primes currently stored   */
static size_t   prime_cap = 0;      /* allocated capacity                   */

/* Ensure that at least `need` primes are available. */
static void
ensure_primes(size_t need)
{
    if (prime_cnt >= need) return;          /* already enough            */

    /* Allocate or grow the array to hold `need` primes. */
    if (prime_cnt == 0) {
        prime_cap = (need > 64) ? need : 64;
        primes = (uint64_t *)malloc(sizeof(uint64_t) * prime_cap);
    } else if (prime_cnt + 1 >= prime_cap) {
        /* Grow capacity by factor 2 when needed. */
        prime_cap = prime_cap * 2;
        primes = (uint64_t *)realloc(primes, sizeof(uint64_t) * prime_cap);
    }

    uint64_t candidate = (prime_cnt == 0) ? 2 : primes[prime_cnt - 1] + 1;
    while (prime_cnt < need) {
        bool prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = primes[i];
            if (p * p > candidate) break;
            if (candidate % p == 0) { prime = false; break; }
        }
        if (prime) {
            if (prime_cnt >= prime_cap) {    /* safety check                    */
                prime_cap *= 2;
                primes = (uint64_t *)realloc(primes, sizeof(uint64_t) * prime_cap);
            }
            primes[prime_cnt++] = candidate;
        }
        ++candidate;
    }
}

/* ----- Sequence function ------------------------------------------------- */
seq_t a(idx_t n)
{
    /* a(1) is defined to be 1. */
    if (n == 1) return (seq_t)1;

    /* For n >= 2 we need the first (n-1) primes. */
    ensure_primes((size_t)(n - 1));

    if (n == 2) {
        /* a(2) = A008578(1)+A008578(2) = 1 + 2. */
        return (seq_t)1 + (seq_t)primes[0];
    }

    /* For n >= 3: a(n) = primes[n-3] + primes[n-2]   (0‑based indices). */
    return (seq_t)primes[n - 3] + (seq_t)primes[n - 2];
}
