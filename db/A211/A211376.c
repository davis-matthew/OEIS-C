/*
 * A211376  a(n) is the smallest 5-smooth number k such that both prime(n) - k
 *          and prime(n) + k are prime.
 * https://oeis.org/A211376
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 6716, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9716 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A211376
   a(n) = smallest 5‑smooth integer k such that prime(n)-k and prime(n)+k are prime.
   (Offset 3: a(3) is the first term.) */

#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
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

static seq_t smallest_5smooth_k(seq_t p)
{
    /* Generate 5‑smooth numbers (Hamming numbers) in increasing order.
       Store them in a growing array.  The first element is 1.  */
    size_t cap = 1024;
    seq_t *h = mem_new(seq_t, cap);
    if (!h) return 0;              /* allocation failed – unlikely in tests */

    h[0] = 1;
    size_t cnt = 1;                /* number of entries in h */
    size_t i2 = 0, i3 = 0, i5 = 0;
    seq_t next2 = 2, next3 = 3, next5 = 5;

    while (1) {
        seq_t nxt = next2;
        if (next3 < nxt) nxt = next3;
        if (next5 < nxt) nxt = next5;

        /* Further candidates are >= p, thus impossible to satisfy prime-p‑k>0 */
        if (nxt >= p) {
            break;
        }

        /* Append nxt to the list, expanding array if needed. */
        if (cnt == cap) {
            cap <<= 1;
            seq_t *tmp = mem_resize(seq_t, h, cap);
            if (!tmp) { mem_free(h); return 0; }  /* allocation failed */
            h = tmp;
        }
        h[cnt++] = nxt;

        /* Test whether p-nxt and p+nxt are prime. */
        if (isPrimeGeneric(p - nxt) && isPrimeGeneric(p + nxt)) {
            mem_free(h);
            return nxt;   /* found the smallest 5‑smooth k */
        }

        /* Advance pointers for the next Hamming number. */
        if (nxt == next2) { i2++; next2 = h[i2] * 2; }
        if (nxt == next3) { i3++; next3 = h[i3] * 3; }
        if (nxt == next5) { i5++; next5 = h[i5] * 5; }
    }

    mem_free(h);
    return 0;   /* should not happen for n ≥ 3 because of the conjecture */
}

seq_t a(idx_t n)
{
    if (n < 3) return 0;                /* undefined for n < 3 */

    /* Obtain the n‑th prime.  For n ≤ 10000 it fits in 32 bits. */
    uint32_t p32 = get_prime(n);
    seq_t p = (seq_t)p32;

    return smallest_5smooth_k(p);
}
