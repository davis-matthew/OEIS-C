/*
 * A192324  Sequence of numbers formed as remainder of Mersenne numbers
 *          divided by primes.
 * https://oeis.org/A192324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^n - 1) mod prime(n)
 * Range:     u8 n <= 54, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Validated: partial, pass, 72 terms through n = 72 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>

/* modular exponentiation using 128‑bit intermediate.
   Works for any seq_t. */
static seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod)
{
    seq_t res = 1 % mod;
    base   %= mod;
    while (exp) {
        if (exp & 1)
            res = (unsigned __int128)res * base % mod;
        exp >>= 1;
        if (exp)
            base = (unsigned __int128)base * base % mod;
    }
    return res;
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

/* Compute the n-th term of OEIS sequence A192324.
   The index n is 1‑based; a(1) = 1, a(2) = 0, …  */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n = 0 – return 0 defensively */
        return 0;

    /* Obtain the n-th prime (1‑based).  The helper returns a 32‑bit value. */
    uint32_t p = get_prime(n);

    /* Compute 2^n modulo p.  mod_pow returns a value in the same type as the
       modulus, which is safe for all supported seq_t sizes. */
    seq_t powmod = mod_pow((seq_t)2, n, (seq_t)p);

    /* The remainder of (2^n - 1) divided by p.
       Avoid underflow when powmod == 0 (happens only when p == 2). */
    seq_t result;
    if (powmod == (seq_t)0)
        result = (seq_t)p - (seq_t)1;   /* (p - 1) mod p  ==  -1 mod p */
    else
        result = powmod - (seq_t)1;

    return result;
}
