/*
 * A086001  For p = prime(n), a(n) is the smallest k such that p*(1 + 2
 *          Ord(4,p) k) is a base-2 pseudoprime.
 * https://oeis.org/A086001
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest k ≥ 1 such that for p = prime(n) p*(1 +
 *            2*Ord(4,p)*k) is a composite base‑2 pseudoprime. Offset: 2 (a(2)
 *            is the first term listed)
 * Range:     u8 none, u16 n <= 22, u32 n <= 1136, u64 n <= 10001,
 *            u128 n <= 10001, big any n
 * Validated: standard, pass, 105 terms through n = 9158 (2026-11-03)
 * Helpers:   factorize, is_prime_u64, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "../utils/factorize.h"
#include "../utils/powmod.h"
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

/* Compute the multiplicative order of 4 modulo the odd prime p.
   The function returns the order as a seq_t (small enough to fit
   in the integer representation of seq_t). */
static seq_t order_of_4(uint64_t p)
{
    uint64_t ord = p - 1;                      /* start with phi(p) */
    uint64_t phi = ord;

    uint64_t fac[64];
    int exp[64];
    size_t cnt = factorize(phi, fac, exp);

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t prime = fac[i];
        while ((ord % prime == 0) &&
               (powmod((seq_t)4, ord / prime, (seq_t)p) == (seq_t)1))
            ord /= prime;
    }
    return (seq_t)ord;
}

/* Core sequence function. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;                     /* undefined for n < 2 */

    seq_t p = get_prime(n);                  /* nth prime (1‑based) */
    seq_t t = 2 * order_of_4((uint64_t)p);   /* 2 * Ord(4,p) */

    seq_t k = 1;
    while (1) {
        seq_t q = 1 + t * k;                 /* q = 1 + t*k */
        seq_t m = p * q;                     /* m = p*(1 + t*k) */

        /* m must be composite and a base‑2 pseudoprime */
        if (!is_prime_big(m)) {
            /* exponent fits into 64‑bit for the intended range */
            uint64_t exp = (uint64_t)m - 1;
            if (powmod((seq_t)2, exp, m) == (seq_t)1)
                return k;
        }
        ++k;
    }
}
