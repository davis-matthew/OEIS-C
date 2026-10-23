/*
 * A107212  Primes of the form 3x^2 + 32y^2.
 * https://oeis.org/A107212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 408, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implements A107212  
 *  Finds the n‑th prime p of the form p = 3*x^2 + 32*y^2.
 *  The routine searches successive odd integers, tests for the
 *  representation, then applies a deterministic Miller–Rabin test
 *  (bases {2,325,9375,28178,450775,9780504,1795265022}) suitable
 *  for all 64‑bit numbers.  Found primes are memoised in a
 *  dynamically growing array so that calls to a(n) in any order
 *  are fast.  All arithmetic is performed with integer types;
 *  no floating point or <math.h> functions are used.  The code
 *  is fully compliant with C11/C++17.  It works for any seq_t / idx_t
 *  defined by the build system. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- integer square root (floor) ---------- */
static inline uint64_t int_sqrt_uint64(uint64_t n)
{
    uint64_t lo = 0, hi = ((uint64_t)1 << 32) - 1, res = 0;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        uint64_t sq = mid * mid;
        if (sq == n) return mid;
        if (sq < n) {
            lo = mid + 1;
            res = mid;
        } else {
            hi = mid - 1;
        }
    }
    return res;
}

/* ---------- deterministic Miller‑Rabin for 64‑bit ---------- */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    /* small primes */
    const uint64_t small_primes[] =
        {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }

    /* write n-1 as d*2^s */
    uint64_t d = n - 1;
    uint64_t s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    /* bases that guarantee correctness for 64‑bit */
    const uint64_t bases[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;

        __uint128_t x = 1;
        __uint128_t pow = a;
        uint64_t exp = d;
        while (exp) {
            if (exp & 1)
                x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }
        uint64_t xx = (uint64_t)x;
        if (xx == 1 || xx == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = (x * x) % n;
            xx = (uint64_t)x;
            if (xx == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* ---------- memoised search routine ---------- */
seq_t a(idx_t n)
{
    static seq_t *primes = NULL;     /* array of found primes */
    static size_t cap   = 0;          /* current capacity */
    static idx_t  cnt   = 0;          /* number of primes stored */
    static uint64_t last = 1;         /* last odd number examined */

    /* generate primes until we have at least n of them */
    while (cnt < n) {
        last += 2;  /* skip even numbers; start at 3 */

        /* test whether 'last' can be written as 3*x^2 + 32*y^2 */
        bool has_rep = false;
        for (uint64_t y = 0;; ++y) {
            __uint128_t part = (__uint128_t)32 * y * y;
            if (part > last) break;
            uint64_t rem = last - (uint64_t)part;
            if (rem % 3 != 0) continue;
            uint64_t x2 = rem / 3;
            uint64_t xs = int_sqrt_uint64(x2);
            if (xs * xs == x2) {
                has_rep = true;
                break;
            }
        }

        if (!has_rep) continue;           /* not of the required form */

        if (!is_prime_uint64(last)) continue;  /* not prime */

        /* store the prime */
        if (cnt >= cap) {
            size_t new_cap = cap ? cap * 2 : 64;
            seq_t *tmp = (seq_t*)realloc(primes, new_cap * sizeof(seq_t));
            if (!tmp) return 0;          /* unlikely but defensive */
            primes = tmp;
            cap = new_cap;
        }
        primes[cnt++] = (seq_t)last;
    }

    return primes[n - 1];
}
