/*
 * A107020  Primes p such that 2p+1, 4p+3, 6p+5 are all primes.
 * https://oeis.org/A107020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 37, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the OEIS sequence A107020:
   a(n) = the n-th prime p such that 2p+1, 4p+3 and 6p+5 are all prime.
   The implementation uses trial division by small primes and deterministic
   Miller–Rabin for 64‑bit integers.  Memoisation is performed so that
   calls in arbitrary order are efficient.  For C++ builds a std::vector
   is used; for pure C a malloc/realloc based array is employed.  The
   code compiles under C11 and C++17. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/*--------------------------------------------------------------------
   Prime test for 64‑bit integers
--------------------------------------------------------------------*/
static bool is_prime(uint64_t n)
{
    /* small primes used for trial division */
    static const uint64_t small_primes[] =
        {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL};

    if (n < 2) return false;
    for (size_t i = 0; i < sizeof small_primes / sizeof *small_primes; ++i) {
        uint64_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    /* write n-1 = d*2^s with d odd */
    uint64_t d = n - 1;
    uint32_t s = 0;
    while ((d & 1ULL) == 0ULL) {
        d >>= 1;
        ++s;
    }

    /* deterministic bases for 64‑bit integers */
    static const uint64_t bases[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};

    for (size_t i = 0; i < sizeof bases / sizeof *bases; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0ULL) continue;

        /* compute a^d mod n */
        __uint128_t x = 1;
        __uint128_t base = a;
        uint64_t exp = d;
        while (exp) {
            if (exp & 1ULL) x = (x * base) % n;
            base = (base * base) % n;
            exp >>= 1ULL;
        }

        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint32_t r = 1; r < s; ++r) {
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

/*--------------------------------------------------------------------
   Storage for discovered sequence terms
--------------------------------------------------------------------*/
#ifdef __cplusplus
static std::vector<seq_t> seq_cache;
#else
static seq_t *seq_cache = NULL;
static size_t seq_cnt = 0;
static size_t seq_cap = 0;
#endif

static void ensure_capacity(size_t need)
{
#ifdef __cplusplus
    if (seq_cache.size() < need) seq_cache.reserve(need);
#else
    if (seq_cache == NULL) {
        seq_cap = 16;
        seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    }
    while (seq_cap < need) {
        seq_cap <<= 1;
        seq_cache = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
    }
#endif
}

/*--------------------------------------------------------------------
   Main function
--------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset 1 – behaviour for n<1 irrelevant */

    /* current number of cached terms */
#ifdef __cplusplus
    size_t cached = seq_cache.size();
#else
    size_t cached = seq_cnt;
#endif

    /* mutable state: the last integer we tested for primality          */
    static uint64_t last_tested = 1ULL;
    /* flag to indicate whether we are still generating primes */
    static bool initialized = false;

    /* generate terms until we have at least n of them */
    while (cached < (size_t)n) {
        /* find next prime p */
        do {
            /* advance to next candidate */
            last_tested += (last_tested <= 2ULL ? 1ULL : 2ULL);
            if (last_tested == 2ULL) { /* 2 is a prime */
                break;
            }
            if ((last_tested & 1ULL) == 0ULL) last_tested++;
        } while (!is_prime(last_tested));

        uint64_t p = last_tested;

        /* compute the three expressions */
        uint64_t v1 = 2ULL * p + 1ULL;
        uint64_t v2 = 4ULL * p + 3ULL;
        uint64_t v3 = 6ULL * p + 5ULL;

        if (is_prime(v1) && is_prime(v2) && is_prime(v3)) {
            /* store as seq_t */
#ifdef __cplusplus
            seq_cache.push_back((seq_t)p);
#else
            ensure_capacity(seq_cnt + 1);
            seq_cache[seq_cnt++] = (seq_t)p;
#endif
            ++cached;
        }
    }

    /* return the requested element (1‑based) */
#ifdef __cplusplus
    return seq_cache[n - 1];
#else
    return seq_cache[n - 1];
#endif
}
