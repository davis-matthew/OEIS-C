/*
 * A139758  a(n) is the smallest prime such that (a(n) - the n-th prime) is a
 *          power of 2.
 * https://oeis.org/A139758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000040(n) + 2^m, where m is the smallest integer such
 *            that the sum is prime
 * Range:     u8 n <= 17, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-03)
 * Helpers:   build_primes, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation retrieves the n‑th prime and increments it by successive
   powers of two until a prime is found.  The prime table is cached and
   expanded as needed using _prime_limit_for_idx to bound the search. */

#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <stddef.h>
#include "../utils/build_primes.h"
#include "../utils/is_prime_u64.h"

static size_t _prime_limit_for_idx(idx_t n) {
    /* Rough upper bound for the n-th prime using n*(log n + log log n) + 3 */
    if (n < 6) return 15; /* first 6 primes < 15 */
    double nn = (double)n;
    double bound = nn * (log(nn) + log(log(nn))) + 3.0;
    return (size_t)bound;
}

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Cached prime list: 1‑based, prime_cache[0] is the first prime. */
static idx_t *prime_cache = NULL;
static size_t prime_cache_cnt = 0;

/* Return the n‑th prime (1‑based).  Builds new primes up to an
   upper bound for n if necessary. */
static idx_t nth_prime(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n=0, not used */

    /* If already cached enough primes, return directly. */
    if (n <= prime_cache_cnt)
        return prime_cache[n - 1];

    /* Obtain an upper bound for the n‑th prime. */
    idx_t limit = _prime_limit_for_idx(n);

    /* Allocate a new prime array up to the requested limit. */
    idx_t *new_cache = NULL;
    size_t new_cnt = 0;
    build_primes(limit, &new_cache, &new_cnt);

    /* Replace old cache (free the old array). */
    free(prime_cache);
    prime_cache = new_cache;
    prime_cache_cnt = new_cnt;

    return prime_cache[n - 1];
}

/* Main term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* below offset, not defined */

    /* Obtain the n‑th prime. */
    idx_t p = nth_prime(n);

    /* Start with 2^0 = 1 and keep doubling until the sum is prime. */
    uint64_t delta = 1;           /* 2^k, starts at k=0 */
    while (1) {
        uint64_t cand = p + delta;
        /* Be defensive against overflow; the sequence only requires values
           up to 10000th term, well below 2^64, so this is just a guard. */
        if (cand < p) break;  /* overflow detected */

        /* Test primality; is_prime_big accepts seq_t, so cast. */
        if (is_prime_big((seq_t)cand))
            return (seq_t)cand;

        /* Prepare next power of two; stop if shift would overflow. */
        if (delta > (UINT64_MAX >> 1))
            break;  /* no further powers fit in uint64_t */

        delta <<= 1;
    }

    /* Should never reach here for valid n. Return 0 as a safe fallback. */
    return (seq_t)0;
}
