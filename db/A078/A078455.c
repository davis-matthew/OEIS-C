/*
 * A078455  a(1) = 2. For n>1, a(n) = smallest m such that m == 0 (mod
 *          prime(n)), m + 1 == 0 (mod prime(n+1)) and m-1 == 0 (mod
 *          prime(n-1)).
 * https://oeis.org/A078455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Validated: partial, pass, 34 terms through n = 34 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A078455: for n>1, the smallest m such that
 *   m      ≡ 0   (mod prime(n)),
 *   m + 1  ≡ 0   (mod prime(n+1)),
 *   m – 1  ≡ 0   (mod prime(n‑1)),
 * and a(1) = 2.
 * The computation uses the Chinese remainder theorem.  */

#include <stdint.h>
#include <stdbool.h>

/* Utility that returns the n-th prime (n>=1).  */
#include <stdlib.h>
#include <stddef.h>

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

/* Extended‑Euclid to compute the modular inverse of a modulo mod
 * (both are 64‑bit and mod is prime).  */
static inline uint64_t inv_mod_uint64(uint64_t a, uint64_t mod)
{
    int64_t t = 0, newt = 1;
    uint64_t r = mod, newr = a % mod;
    while (newr != 0) {
        uint64_t q = r / newr;
        int64_t tmp = t - (int64_t)q * newt;
        t = newt;  newt = tmp;
        tmp = r - q * newr;
        r = newr;  newr = tmp;
    }
    if (r > 1) return 0;           /* not invertible – should not happen */
    if (t < 0) t += (int64_t)mod;
    return (uint64_t)t;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;   /* base case */

    /* retrieve the three consecutive primes */
    uint32_t p_prev = get_prime(n - 1);  /* prime(n-1) */
    uint32_t p      = get_prime(n);      /* prime(n)   */
    uint32_t p_next = get_prime(n + 1);  /* prime(n+1) */

    /* residues for the congruences, in the order we will combine them */
    uint64_t mods[3]     = {(uint64_t)p, (uint64_t)p_next, (uint64_t)p_prev};
    uint64_t residues[3] = {0ULL,          (uint64_t)p_next - 1ULL,
                            1ULL};

    seq_t result  = 0;
    seq_t mod_prod = 1;               /* product of processed moduli (seq_t) */

    for (int i = 0; i < 3; ++i) {
        uint64_t mod = mods[i];
        uint64_t residue = residues[i];

        /* current remainder of the result modulo mod */
        uint64_t r_mod = (uint64_t)(result % mod);
        /* difference needed to reach the desired residue */
        uint64_t diff = (residue + mod - r_mod) % mod;

        /* inverse of current modulus product modulo mod */
        uint64_t inv = inv_mod_uint64((uint64_t)(mod_prod % mod), mod);
        uint64_t t   = (diff * inv) % mod;

        /* update the result and accumulated modulus product */
        result += (seq_t)t * mod_prod;
        mod_prod *= (seq_t)mod;
    }

    return result;   /* smallest non‑negative solution */
}
