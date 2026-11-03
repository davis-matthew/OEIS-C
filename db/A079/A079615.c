/*
 * A079615  Product of all distinct prime factors of all composite numbers
 *          between n-th prime and next prime.
 * https://oeis.org/A079615
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = product of distinct primes appearing in any composite
 *            number between the n-th prime and the next prime. The
 *            implementation uses prime factorization of each composite in
 *            that interval and keeps a set of primes already multiplied
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 29, u64 n <= 46, u128 n <= 153,
 *            big any n
 * Validated: standard, pass, 50 terms through n = 984 (2026-11-03)
 * Helpers:   factor_u64, is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime_u128.h"
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

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;  /* undefined for n < 2 per OEIS offset */

    /* Get the interval [p+1, q-1] where p = n-th prime, q = (n+1)-th prime */
    idx_t p = get_prime(n);
    idx_t q = get_prime(n + 1);

    seq_t result = 1;

    /* Keep track of primes already multiplied */
    uint64_t seen[128];
    size_t seen_cnt = 0;

    /* Buffers for factorization */
    uint64_t prime_buf[64];
    uint64_t exp_buf[64];   /* not used, but required by factor_u64 */
    size_t cnt;

    for (idx_t m = p + 1; m < q; ++m) {
        /* Skip primes themselves */
        if (isPrimeGeneric((seq_t)m)) continue;

        /* Factor m and get its distinct prime factors */
        factor_u64((uint64_t)m, prime_buf, exp_buf, &cnt);

        for (size_t i = 0; i < cnt; ++i) {
            uint64_t pf = prime_buf[i];
            bool already = false;
            for (size_t j = 0; j < seen_cnt; ++j) {
                if (seen[j] == pf) { already = true; break; }
            }
            if (!already) {
                /* Multiply result by this new prime factor */
                result *= (seq_t)pf;
                seen[seen_cnt++] = pf;
            }
        }
    }

    return result;
}
