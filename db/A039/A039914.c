/*
 * A039914  Smallest k>1 such that k(p-1)-1 is divisible by p^2, p=n-th prime.
 * https://oeis.org/A039914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the minimal k > 1 such that k*(p-1)-1 is divisible by p^2,
   where p is the n-th prime.  That k equals the modular inverse of
   (p-1) modulo p^2 (with 1 → p^2 added). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
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

/* extended Euclidean algorithm for int64_t */
static void egcd(int64_t a, int64_t b, int64_t *x, int64_t *y)
{
    if (b == 0) {
        *x = 1;
        *y = 0;
        return;
    }
    int64_t x1, y1;
    egcd(b, a % b, &x1, &y1);
    *x = y1;
    *y = x1 - (a / b) * y1;
}

/* modular inverse of a modulo m (gcd(a,m)=1) */
static uint64_t mod_inv_uint64(uint64_t a, uint64_t m)
{
    int64_t x, y;
    egcd((int64_t)a, (int64_t)m, &x, &y);
    int64_t inv = x % (int64_t)m;
    if (inv < 0)
        inv += (int64_t)m;
    return (uint64_t)inv;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for offset <1 */

    /* obtain the n-th prime, fits in uint32_t for n up to 10000 */
    uint32_t p = get_prime(n);

    /* modulus m = p^2 fits in 64-bit for p <= 104729 (n=10000) */
    uint64_t m = (uint64_t)p * (uint64_t)p;
    uint64_t a_val = (uint64_t)p - 1;

    /* inverse of (p-1) modulo p^2 */
    uint64_t inv = mod_inv_uint64(a_val, m);

    /* k must be > 1, so if inverse equals 1, add modulus */
    if (inv <= 1)
        inv += m;

    return (seq_t)inv;
}
