/*
 * A054510  Log_b 7 where b is smallest primitive root (A001918) mod n-th
 *          prime.
 * https://oeis.org/A054510
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = discrete logarithm of 7 to the base b modulo the n-th
 *            prime p, where b is the smallest primitive root modulo p (OEIS
 *            A001918). Offset = 5. For n < 5 a(n) = 0
 * Range:     u8 n <= 54, u16 n <= 6634, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 9689 (2026-11-01)
 * Helpers:   isqrt, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "../utils/isqrt.h"
#include "../utils/powmod.h"

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

static inline uint64_t mul_mod64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__int128)a * b % mod);
}

/* Smallest primitive root modulo a prime p (p > 2).  Input and output
   are seq_t; inside we use 64‑bit arithmetic because p ≤ 2^64 and
   for the limits of this problem p < 200 000. */
static seq_t smallest_primitive_root(seq_t p)
{
    uint64_t prime = (uint64_t)p;
    uint64_t phi = prime - 1;

    /* factor phi into distinct prime factors */
    uint64_t pf[32];
    size_t nf = 0;
    uint64_t t = phi;
    for (uint64_t q = 2; q * q <= t; ++q) {
        if (t % q == 0) {
            pf[nf++] = q;
            while (t % q == 0) t /= q;
        }
    }
    if (t > 1) pf[nf++] = t;

    /* test candidates g = 2,3,... until a primitive root is found */
    for (uint64_t g = 2; g < prime; ++g) {
        bool ok = true;
        for (size_t i = 0; i < nf && ok; ++i) {
            uint64_t exp = phi / pf[i];
            if (powmod((seq_t)g, exp, p) == 1) ok = false;
        }
        if (ok) return (seq_t)g;
    }
    return (seq_t)prime;   /* never reached for a prime > 2 */
}

/* Baby‑step giant‑step discrete logarithm: find x such that
   g^x ≡ target (mod m).  All arguments are seq_t; internally we
   work with 64‑bit values because m ≤ 200 000. */
static seq_t discrete_log(seq_t g, seq_t target, seq_t m)
{
    uint64_t mod = (uint64_t)m;
    uint64_t gval = (uint64_t)g;
    uint64_t tval = (uint64_t)target;

    uint64_t order = mod - 1;                         /* group order */
    uint64_t bsize = isqrt(order) + 1;     /* ceil sqrt */

    /* store baby steps: key → exponent */
    enum {HT_SIZE = 2048};
    struct entry {
        uint64_t key;
        uint32_t val;
        bool used;
    };
    static struct entry table[HT_SIZE];

    /* reset table */
    for (size_t i = 0; i < HT_SIZE; ++i) table[i].used = false;

    /* baby steps: g^j */
    uint64_t cur = 1;
    for (uint64_t j = 0; j < bsize; ++j) {
        size_t idx = cur & (HT_SIZE - 1);
        while (table[idx].used) idx = (idx + 1) & (HT_SIZE - 1);
        table[idx].key = cur;
        table[idx].val = (uint32_t)j;
        table[idx].used = true;
        cur = mul_mod64(cur, gval, mod);
    }

    /* g^(-bsize) = g^(order - bsize) mod m */
    seq_t factor = powmod(g, order - bsize, m);
    uint64_t g_inv = (uint64_t)factor;

    /* giant steps: target * (g^-bsize)^i */
    uint64_t gamma = tval;
    for (uint64_t i = 0; i < bsize; ++i) {
        size_t idx = gamma & (HT_SIZE - 1);
        while (table[idx].used && table[idx].key != gamma)
            idx = (idx + 1) & (HT_SIZE - 1);
        if (table[idx].used && table[idx].key == gamma) {
            uint64_t j = table[idx].val;
            return (seq_t)(i * bsize + j);
        }
        gamma = mul_mod64(gamma, g_inv, mod);
    }
    return (seq_t)0;    /* should never happen for a valid input */
}

seq_t a(idx_t n)
{
    if (n < 5) return (seq_t)0;
    seq_t p = get_prime(n);                 /* the n‑th prime */
    seq_t g = smallest_primitive_root(p);   /* smallest primitive root modulo p */
    return discrete_log(g, (seq_t)7, p);     /* log_g 7 (mod p) */
}
