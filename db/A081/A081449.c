/*
 * A081449  Smallest squares such that partial sums of the sequence plus 17
 *          are primes.
 * https://oeis.org/A081449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A081449 – smallest squares (6m)^2 chosen greedily so that
 * each partial sum plus 17 is prime.  In other words,
 *
 *   a(1) = 36
 *   for n > 1, a(n) is the smallest square of the form (6m)^2 that
 *   satisfies:  (a(1)+…+a(n-1)+a(n)+17) is prime, chosen with the
 *   smallest possible m.  The search restarts from m = 1 for every
 *   term, so the same square may occur repeatedly.
 *
 * The implementation uses a cache of previously computed terms and
 * a deterministic 64‑bit Miller–Rabin test for primality.  For the
 * arbitrary‑precision build (seq_t ≥ 128) the running sum still fits
 * comfortably in 64 bits for the required range, so the primality test
 * operates on a 64‑bit value cast from the sum.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------------- */
/* 64‑bit deterministic Miller–Rabin primality test                          */
/* ------------------------------------------------------------------------- */
static bool is_prime64(uint64_t n)
{
    if (n < 2)                  return false;
    if (n < 4)                  return true;   /* 2 or 3 */
    if ((n & 1) == 0)           return false;

    /* write n-1 as d * 2^s with d odd */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* Deterministic bases for 64‑bit integers */
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a <= 1) continue;

        /* modular exponentiation a^d mod n */
        __uint128_t x = 1;
        __uint128_t p = a;
        uint64_t exp = d;
        while (exp) {
            if (exp & 1) x = (x * p) % n;
            p = (p * p) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n - 1) continue;

        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* ------------------------------------------------------------------------- */
/*                           Sequence generation                               */
/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Persistent cache: a[0] = a(1), a[1] = a(2), … */
    static seq_t *cache    = NULL;    /* array of terms */
    static idx_t  size     = 0;       /* number of stored terms */
    static idx_t  capacity = 0;       /* allocated array size */
    static seq_t  sum      = 0;       /* running sum of terms */

    if (n == 0) return 0;  /* offset is 1 – undefined for n = 0 */

    /* Generate terms until the cache contains at least n entries */
    while (size < n) {
        /* Search for the smallest m such that (6m)^2 keeps the sum prime */
        for (seq_t m = 1;; ++m) {
            seq_t square = 36ULL * m * m;   /* (6m)^2 */
            uint64_t test = (uint64_t)(sum + square) + 17ULL;
            if (is_prime64(test)) {
                /* Store the found term */
                if (size == capacity) {
                    capacity = capacity ? capacity << 1 : 4;
                    cache = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
                }
                cache[size++] = square;
                sum += square;
                break;  /* move on to the next term */
            }
        }
    }

    return cache[n - 1];
}
