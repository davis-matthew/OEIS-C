/*
 * A073703  Smallest prime p such that also p+prime(n)*2 is a prime.
 * https://oeis.org/A073703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 3821, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 109 terms through n = 9707 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This function implements the OEIS definition for A073703:
    a(n) = least prime p such that p + 2*prime(n) is prime.
    It searches over primes, expanding the prime sieve as needed.
*/

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_u64.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
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

static uint64_t *shared_primes   = NULL; /* array of primes */
static size_t   shared_prime_cnt = 0;
static idx_t    shared_limit     = 0;    /* current sieve upper bound */

/* Rebuild prime list up to at least <limit> if required */
static void ensure_primes_upto(idx_t limit)
{
    if (shared_limit >= limit)
        return;

    if (shared_primes) free(shared_primes);
    build_primes_upto(limit, &shared_primes, &shared_prime_cnt);
    shared_limit = limit;
}

/*------------------------------------------------------------------*/
/* Main sequence function                                            */
/*------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* 1. Get the n-th prime (OEIS indexing starts at 1) */
    uint32_t pn = get_prime(n);          /* prime(n)  */

    /* 2. Compute offset 2*prime(n) */
    seq_t d = (seq_t)pn * 2U;

    /* 3. Ensure we have a prime list ready (start with 1 000 000) */
    if (shared_limit == 0)
        ensure_primes_upto((idx_t)1000000ULL);

    /* 4. Search for the smallest prime p with p + d prime */
    for (;;)
    {
        for (size_t i = 0; i < shared_prime_cnt; ++i)
        {
            seq_t p   = (seq_t)shared_primes[i];
            seq_t sum = p + d;
            if (is_prime_big(sum))
                return p;
        }

        /* No match within current sieve, double the limit and rebuild. */
        idx_t new_limit = shared_limit * (idx_t)2;
        ensure_primes_upto(new_limit);
    }
}
