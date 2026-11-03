/*
 * A180100  a(n) = smallest number k such that five consecutive prime numbers
 *          prime(n), prime(n+1), prime(n+2), prime(n+3) and prime(n+4) are
 *          divisors of k, k+1, k+2, k+3, and k+4 respectively.
 * https://oeis.org/A180100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = smallest k such that prime(n) | k, * prime(n+1) | k+1,
 *            * prime(n+2) | k+2, * prime(n+3) | k+3, * prime(n+4) | k+4. *
 *            The value is the unique solution modulo the product of the five
 *            primes. * We compute it by the Chinese‑Remainder Theorem
 * Range:     u8 none, u16 n <= 3, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 23 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

/* modular inverse of a modulo mod (mod is prime, a < mod) */
static uint64_t modinv_uint64(uint64_t a, uint64_t mod)
{
    int64_t t = 0, newt = 1;
    int64_t r = mod, newr = a;
    while (newr != 0) {
        uint64_t q = r / newr;
        int64_t tmp = newt;
        newt = t - (int64_t)((__int128)q * newt);
        t = tmp;
        tmp = newr;
        newr = r - q * newr;
        r = tmp;
    }
    if (r > 1) return 0;                  /* no inverse – shouldn’t happen */
    if (t < 0) t += mod;
    return (uint64_t)t;
}

seq_t a(idx_t n)
{
    /* fetch five consecutive primes; indices are 1‑based */
    uint64_t p[5];
    p[0] = get_prime(n);
    p[1] = get_prime(n + 1);
    p[2] = get_prime(n + 2);
    p[3] = get_prime(n + 3);
    p[4] = get_prime(n + 4);

    /* residues r[i] = -i (mod p[i]) */
    uint64_t r[5];
    for (int i = 0; i < 5; ++i)
        r[i] = (p[i] - (uint64_t)i) % p[i];

    seq_t cur = 0;        /* current solution modulo processed primes */
    seq_t mod = 1;        /* product of processed primes */

    for (int i = 0; i < 5; ++i) {
        /* product of previous primes modulo p[i] */
        uint64_t prod = 1 % p[i];
        for (int j = 0; j < i; ++j)
            prod = (prod * p[j]) % p[i];

        uint64_t inv = modinv_uint64(prod, p[i]);

        uint64_t cur_mod = (uint64_t)(cur % (seq_t)p[i]);

        uint64_t diff = (r[i] >= cur_mod) ? (r[i] - cur_mod)
                                           : (r[i] + p[i] - cur_mod);

        uint64_t t = (uint64_t)(((unsigned __int128)diff * inv) % p[i]);

        cur = cur + mod * (seq_t)t;
        mod = mod * (seq_t)p[i];
    }

    return cur;   /* minimal non‑negative solution */
}
