/*
 * A059940  Smallest prime p such that x = n is a solution mod p of x^3 = 2,
 *          or 0 if no such prime exists.
 * https://oeis.org/A059940
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A059940
 *  Formula: a(n) = the smallest prime factor of n³ – 2 that is > n;
 *  if no such prime exists, a(n) = 0.
 *  Indexing starts at n = 2; values for n < 2 are defined as 0.
 *
 *  The function factors n³‑2 by trial division using a pre‑computed list of
 *  primes up to 1 000 000, which is sufficient for all n up to about 10 000
 *  (n³ – 2 ≤ 10¹²).  The algorithm works for any build of seq_t that
 *  supports the ordinary arithmetic operators and comparison.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>   /* for size_t */

/* Pre‑computed primes up to 1 000 000.  The number of primes < 1 000 000
 * is 78 498, which fits comfortably in the static array below. */
#define PRIME_LIMIT 1000000
#define MAX_PRIMES 78498
static uint32_t primes[MAX_PRIMES];
static size_t prime_count = 0;
static bool primes_ready = false;

/* Sieve of Eratosthenes – built once on first call. */
static void build_primes(void) {
    if (primes_ready) return;
    static bool marked[PRIME_LIMIT + 1];
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!marked[i]) {
            primes[prime_count++] = i;
            if ((uint64_t)i * i <= PRIME_LIMIT)
                for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                    marked[j] = true;
        }
    }
    primes_ready = true;
}

/* External entry point for the sequence. */
seq_t a(idx_t n) {
    if (n < 2) return 0;          /* below the offset */

    build_primes();                /* ensure the prime list is ready */

    /* Compute m = n³ – 2 using seq_t arithmetic. */
    seq_t nn = (seq_t)n;
    seq_t m  = nn * nn * nn - (seq_t)2;
    if (m <= (seq_t)1) return 0;  /* safety guard */

    seq_t tmp   = m;               /* variable to be divided by prime factors */
    seq_t best  = 0;               /* smallest prime factor > n found so far */

    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p_uint = primes[i];
        seq_t p = (seq_t)p_uint;
        seq_t p2 = p * p;

        /* If p² > tmp, all remaining factors are > sqrt(tmp),
         * so stop the trial‑division loop. */
        if (p2 > tmp) break;

        /* Skip recording factors ≤ n, but still divide them out. */
        if (tmp % p == 0) {
            if (p > (seq_t)n && (best == 0 || p < best))
                best = p;              /* candidate smallest prime > n */

            /* Remove all occurrences of p from tmp. */
            while (tmp % p == 0)
                tmp /= p;
        }
    }

    /* After removing all checked factors, tmp (if >1) is the remaining
     * cofactor.  It may be a prime > sqrt(original m).  If it is > n
     * and smaller than any candidate found above, record it. */
    if (tmp > (seq_t)1 && tmp > (seq_t)n && (best == 0 || tmp < best))
        best = tmp;

    return best;   /* zero means no prime factor > n exists */
}
