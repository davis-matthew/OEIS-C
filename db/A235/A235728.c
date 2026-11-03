/*
 * A235728  a(n) = |{0 < k < n - 2: 2*m + 1, m*(m+1) - prime(m) and m*(m+1) +
 *          prime(m) are all prime with m = phi(k) + phi(n-k)/2}|, where
 *          phi(.) is Euler's totient function.
 * https://oeis.org/A235728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = |{ 0 < k < n-2 : * 2*m+1, m*(m+1)-prime(m) and
 *            m*(m+1)+prime(m) * are all prime, where m = phi(k) + phi(n-k)/2
 *            }| * (phi is Euler's totient, prime(m) is the m-th prime,
 *            1‑indexed)
 * Range:     u8 n <= 19, u16 n <= 375, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9971 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
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

static uint64_t *phi_cache = NULL;   /* phi_cache[i] = φ(i) for 0 <= i <= size */
static size_t    phi_cache_size = 0; /* last computed index (>=0) */

/* recompute the totient table up to 'limit' inclusive (O(limit log log limit)) */
static void compute_phi_cache(size_t limit)
{
    uint64_t *new_cache = (uint64_t *)malloc((limit + 1) * sizeof(uint64_t));
    for (size_t i = 0; i <= limit; ++i)
        new_cache[i] = i;

    for (size_t p = 2; p <= limit; ++p) {
        if (new_cache[p] == p) { /* p is prime */
            for (size_t q = p; q <= limit; q += p) {
                new_cache[q] -= new_cache[q] / p;
            }
        }
    }

    free(phi_cache);
    phi_cache     = new_cache;
    phi_cache_size = limit;
}

/* ensure phi_cache contains values up to 'limit' */
static void ensure_phi_cache(size_t limit)
{
    if (limit > phi_cache_size)
        compute_phi_cache(limit);
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 6)                     /* first five terms are zero */
        return 0;

    ensure_phi_cache((size_t)n);

    seq_t count = 0;
    for (idx_t k = 1; k <= n - 3; ++k) {
        uint64_t t1 = phi_cache[(size_t)k];
        uint64_t t2 = phi_cache[(size_t)(n - k)];
        uint64_t m  = t1 + t2 / 2;        /* integer division – phi(n-k) is even */

        if (m == 0)    /* should never happen for n >= 6, but guard against it */
            continue;

        seq_t m_seq = (seq_t)m;
        seq_t p1    = 2 * m_seq + 1;
        seq_t prime_m = (seq_t)get_prime((idx_t)m);
        seq_t prod     = m_seq * (m_seq + 1);
        seq_t p2       = prod - prime_m;
        seq_t p3       = prod + prime_m;

        if (is_prime_big(p1) && is_prime_big(p2) && is_prime_big(p3))
            ++count;
    }
    return count;
}
