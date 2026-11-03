/*
 * A054505  Log_b 2 where b is smallest primitive root (A001918) mod n-th
 *          prime.
 * https://oeis.org/A054505
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = log_b 2 (mod p), where p = n-th prime (n ≥ 2) and b is
 *            the smallest primitive root modulo p. Returns 0 for n < 2
 *            (outside the OEIS offset)
 * Range:     u8 n <= 54, u16 n <= 6603, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 92 terms through n = 9205 (2026-11-03)
 * Helpers:   isqrt, mem, mulmod, powmod, smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/isqrt.h"
#include "../utils/mulmod.h"
#include "../utils/powmod.h"
#include "../utils/smallest_prime_factor.h"
#include "../utils/mem.h"

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

/* Factor p-1 into its distinct prime factors.
   The array 'factors' must be large enough (≤ 32). */
static void factor_distinct(uint64_t n, uint64_t *factors, size_t *cnt)
{
    *cnt = 0;
    while (n > 1) {
        uint64_t f = smallest_prime_factor(n);
        factors[(*cnt)++] = f;
        while (n % f == 0) n /= f;
    }
}

/* Smallest primitive root modulo prime p (p > 2). */
static seq_t smallest_primitive_root(uint64_t p)
{
    uint64_t factors[32];
    size_t fc;
    factor_distinct(p - 1, factors, &fc);

    for (uint64_t g = 2; g < p; ++g) {
        bool ok = true;
        for (size_t i = 0; i < fc; ++i) {
            uint64_t q = factors[i];
            seq_t exp = (seq_t)((p - 1) / q);
            seq_t res = powmod((seq_t)g, (uint64_t)exp, (seq_t)p);
            if (res == 1) {
                ok = false;
                break;
            }
        }
        if (ok) return (seq_t)g;
    }
    return 0; /* Should never happen for prime p > 2 */
}

/* Discrete logarithm using baby‑step giant‑step.
   Finds k such that base^k ≡ target (mod mod), 0 ≤ k < mod-1. */
static seq_t discrete_log(seq_t base, seq_t target, seq_t mod)
{
    uint64_t p = (uint64_t)mod;           /* p fits in 64 bits */
    uint64_t m = isqrt(p - 1) + 1;

    /* Use a simple open‑addressing hash table. */
    size_t tblsize = 1;
    while (tblsize < 2 * m) tblsize <<= 1;
    size_t mask = tblsize - 1;

    struct entry { seq_t key; idx_t val; } *table =
        mem_new(struct entry, tblsize);
    if (!table) return 0; /* allocation failure, unlikely */

    /* Baby steps: base^j */
    seq_t baby = 1;
    for (size_t j = 0; j < m; ++j) {
        uint64_t h = ((uint64_t)baby * 1315423911ull) & mask;
        while (table[h].key != 0) h = (h + 1) & mask;
        table[h].key = baby;
        table[h].val = (idx_t)j;
        baby = (seq_t)mulmod((uint64_t)baby, (uint64_t)base, p);
    }

    /* Compute base^(-m) mod mod */
    seq_t inv_base_m = powmod(base, (unsigned long)(p - 1 - m), mod);

    seq_t cur = target % mod;
    for (size_t i = 0; i <= m; ++i) {
        uint64_t h = ((uint64_t)cur * 1315423911ull) & mask;
        while (table[h].key != 0) {
            if (table[h].key == cur) {
                seq_t result = (seq_t)(i * m + table[h].val);
                mem_free(table);
                return result;
            }
            h = (h + 1) & mask;
        }
        cur = (seq_t)mulmod((uint64_t)cur, (uint64_t)inv_base_m, p);
    }
    mem_free(table);
    return 0; /* should never happen for a prime modulus */
}

/* Public sequence function required by the build. */
seq_t a(idx_t n)
{
    if (n < 2) return 0; /* outside of OEIS offset */

    uint32_t p32 = get_prime(n);   /* n-th prime, fits in 32 bits */
    uint64_t p  = (uint64_t)p32;
    seq_t    prime = (seq_t)p;

    seq_t g = smallest_primitive_root(p);
    seq_t log2 = discrete_log(g, (seq_t)2u, prime);
    return log2;
}
