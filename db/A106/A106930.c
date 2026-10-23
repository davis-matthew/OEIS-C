/*
 * A106930  Primes of the form x^2 - xy + 16y^2, with x and y nonnegative.
 * https://oeis.org/A106930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 801, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the n‑th prime that can be written as x^2 - x*y + 16*y^2
 *          (equivalently x^2 + 63*y^2) with x,y ≥ 0. This computes the
 *          sequence by sieving primes up to a fixed limit and testing each
 *          for the representation, memoising the results.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* integer square root for uint64_t using 128‑bit arithmetic */
static uint64_t isqrt64(uint64_t n)
{
    uint64_t lo = 0, hi = 0xFFFFFFFFULL; /* sqrt(2^64-1) < 2^32 */
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq == n)
            return mid;
        if (sq < n)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

/* check whether prime p can be written as x^2 - x*y + 16*y^2 (x,y≥0) */
static bool representable_prime(uint64_t p)
{
    if (p < 67)               /* 67 is the smallest such prime */
        return false;

    uint64_t four_p = p << 2;                      /* 4p */
    uint64_t max_y2 = four_p / 63;                 /* y^2 <= 4p/63 */
    uint64_t y_max  = isqrt64(max_y2);

    for (uint64_t y = 1; y <= y_max; ++y) {
        uint64_t y2   = y * y;
        uint64_t rem  = four_p - 63ULL * y2;       /* should be t^2 */
        uint64_t t    = isqrt64(rem);
        if (t * t == rem) {                        /* perfect square? */
            if (((t + y) & 1ULL) == 0)             /* 2x = t + y even? */
                return true;
        }
    }
    return false;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined for offset 0 */

    /* static state for memoisation */
    static bool built = false;
    static seq_t *seq = NULL;
    static size_t seq_size = 0;
    const size_t REQUIRED_TERMS = 20000;   /* enough for n up to ~10k */

    if (!built) {
        /* upper bound for the sieve: 20 million is enough for 20000 terms */
        const uint64_t LIMIT = 20000000ULL;

        /* sieve of Eratosthenes for odd numbers only */
        uint64_t odd_count = (LIMIT - 1) / 2;            /* number of odd ≤ LIMIT */
        size_t sieve_bytes = (odd_count >> 3) + 1;
        uint8_t *sieve = (uint8_t *)calloc(sieve_bytes, 1);
        if (!sieve) exit(EXIT_FAILURE);

        for (uint64_t i = 0, p = 3; p * p <= LIMIT; i++, p += 2) {
            if (!(sieve[i >> 3] & (1U << (i & 7)))) {
                uint64_t start = p * p;
                uint64_t j = (start - 3ULL) >> 1;
                for (; j < odd_count; j += p) {
                    sieve[j >> 3] |= (1U << (j & 7));
                }
            }
        }

        /* allocate storage for the sequence */
        seq = (seq_t *)malloc(REQUIRED_TERMS * sizeof(seq_t));
        if (!seq) exit(EXIT_FAILURE);
        seq_size = 0;

        /* process odd primes; 2 is not representable */
        for (uint64_t i = 0, p = 3; i < odd_count; ++i, p += 2) {
            if (!(sieve[i >> 3] & (1U << (i & 7)))) {    /* prime */
                if (representable_prime(p)) {
                    seq[seq_size++] = (seq_t)p;
                    if (seq_size >= REQUIRED_TERMS)
                        break;
                }
            }
        }

        free(sieve);
        built = true;
    }

    if ((idx_t)seq_size < n)
        return (seq_t)0;    /* out of precomputed range */
    return seq[n - 1];
}
