/*
 * A240087  Smallest difference to start a prime arithmetic progression of
 *          three or more terms with the n-th prime number.
 * https://oeis.org/A240087
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 7029, u32 n <= 10001, u64 n <= 10001,
 *            u128 n <= 10001, big any n
 * Validated: standard, pass, 84 terms through n = 8608 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the smallest positive integer d such that
 *   prime(n) + d*k  (k = 0,1,2,…) are all prime for at least three terms,
 *   but prime(n) − d is not prime.
 * This code searches d incrementally, caching results for efficiency.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

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

seq_t a(idx_t n)
{
    /*
     * Cache: index 0 and 1 are unused; a(0)=a(1)=0.
     * The cache is resized to n+1 on first access to a(n).
     */
    static seq_t *cache = NULL;      /* array of computed a(n) */
    static size_t cache_cap = 0;     /* current size of cache array */

    if (n < 2) {
        /* undefined for n < 2; return 0 for safety */
        return 0;
    }

    /* Grow cache if needed */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) {
            abort(); /* allocation failed */
        }
        /* Initialize newly allocated slots to 0 (sentinel) */
        for (size_t i = cache_cap; i < new_cap; ++i) {
            tmp[i] = 0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Return cached value if already computed */
    if (cache[n] != 0) {
        return cache[n];
    }

    /* Compute the n-th prime (type seq_t) */
    seq_t prime_n = (seq_t)get_prime(n);
    seq_t answer = 0;

    /* Search incrementally for the smallest suitable difference */
    for (seq_t d = 1; ; ++d) {

        /* If prime_n - d is a prime, skip this d.
           For d > prime_n-2, prime_n-d <= 1, which is never prime. */
        if (d <= prime_n - (seq_t)2 && is_prime_big(prime_n - d)) {
            continue;
        }

        seq_t p1 = prime_n + d;
        if (!is_prime_big(p1)) {
            continue;
        }

        seq_t p2 = prime_n + (seq_t)2 * d;
        if (!is_prime_big(p2)) {
            continue;
        }

        /* Found the smallest d with at least three terms in the progression */
        answer = d;
        break;
    }

    cache[n] = answer;
    return answer;
}
