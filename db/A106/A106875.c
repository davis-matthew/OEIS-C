/*
 * A106875  Primes of the form 3x^2+2xy+3y^2, with x and y nonnegative.
 * https://oeis.org/A106875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 639, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th prime of the form 3*x^2 + 2*x*y + 3*y^2.  */
/* The function generates all values ≤ an increasing bound, tests each for */
/* primality and returns the n‑th unique prime found.  */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Mergeable integer type for the test only; 64‑bit is enough for the limits */
static bool is_prime_ull(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof small_primes/sizeof *small_primes; ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }
    /* Write n-1 as d * 2^s  (d odd) */
    uint64_t d = n - 1;
    uint64_t s = 0;
    while ((d & 1) == 0) { d >>= 1; s++; }
    static const uint64_t bases[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof bases/sizeof *bases; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        /* compute a^d mod n */
        unsigned __int128 pow = 1, base = a;
        uint64_t exp = d;
        while (exp) {
            if (exp & 1) pow = (pow * base) % n;
            base = (unsigned __int128)base * base % n;
            exp >>= 1;
        }
        unsigned __int128 x = pow;
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* comparison function for qsort on uint64_t */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return (va > vb) - (va < vb);
}

seq_t a(idx_t n)
{
    /* n is 1‑based; for n==0 behaviour is undefined */
    if (n == 0) return 0;

    uint64_t limit = 100;                     /* initial search bound */
    while (true) {
        /* --- generate all candidates ≤ limit --- */
        uint64_t *cand = NULL;
        size_t cap = 0, cnt = 0;
        /* upper bound on count: (floor(sqrt(limit/3)) + 1)^2  */
        uint64_t max_y = 0;
        {
            uint64_t tmp = limit / 3;
            /* simple integer sqrt */
            uint64_t y = 0;
            while ((y + 1) * (y + 1) <= tmp) y++;
            max_y = y;
        }
        size_t est_cap = (max_y + 1) * (max_y + 1);
        cand = (uint64_t *)malloc(est_cap * sizeof(uint64_t));
        if (!cand) return 0;                 /* memory failure */
        cap = est_cap;

        for (uint64_t y = 0;; ++y) {
            unsigned __int128 val0 = (unsigned __int128)3 * y * y;
            if (val0 > limit) break;
            for (uint64_t x = 0;; ++x) {
                unsigned __int128 val =
                    (unsigned __int128)3 * x * x +
                    (unsigned __int128)2 * x * y +
                    (unsigned __int128)3 * y * y;
                if (val > limit) break;
                if (cnt == cap) {          /* dynamic increase if needed */
                    cap *= 2;
                    cand = (uint64_t *)realloc(cand, cap * sizeof(uint64_t));
                    if (!cand) return 0;
                }
                cand[cnt++] = (uint64_t)val;
            }
        }

        /* sort and unique‑filter while counting primes */
        qsort(cand, cnt, sizeof(uint64_t), cmp_uint64);

        uint64_t prev = 0;
        size_t found = 0;
        for (size_t i = 0; i < cnt; ++i) {
            uint64_t val = cand[i];
            if (i > 0 && val == prev) continue;  /* skip duplicate */
            prev = val;
            if (is_prime_ull(val)) {
                found++;
                if (found == n) {
                    free(cand);
                    return val;
                }
            }
        }

        free(cand);
        /* increase bound and repeat */
        limit <<= 1;           /* double limit */
    }
}
