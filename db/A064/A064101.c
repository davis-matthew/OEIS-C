/*
 * A064101  Primes p = prime(k) such that prime(k) + prime(k+5) = prime(k+1) +
 *          prime(k+4) = prime(k+2) + prime(k+3).
 * https://oeis.org/A064101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(n) = prime(k) where prime(k)+prime(k+5) =
 *            prime(k+1)+prime(k+4) = prime(k+2)+prime(k+3) (1-indexed)
 * Range:     u8 n <= 7, u16 n <= 115, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 59 terms through n = 998 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* External utilities provided by the build environment */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Cache for the computed sequence terms */
static idx_t   _next_k  = 1;   /* next prime index to test (1‑based) */
static seq_t  *_seq_buf = NULL;
static size_t  _seq_len = 0;
static size_t  _seq_cap = 0;

/* Ensure that the cache contains at least 'need' terms */
static void _ensure_cache(idx_t need)
{
    while (_seq_len < need) {
        /* fetch the six consecutive primes starting at _next_k */
        seq_t p0 = (seq_t)get_prime(_next_k);
        seq_t p1 = (seq_t)get_prime(_next_k + 1);
        seq_t p2 = (seq_t)get_prime(_next_k + 2);
        seq_t p3 = (seq_t)get_prime(_next_k + 3);
        seq_t p4 = (seq_t)get_prime(_next_k + 4);
        seq_t p5 = (seq_t)get_prime(_next_k + 5);

        /* compute the three pairwise sums */
        seq_t s1 = p0 + p5;
        seq_t s2 = p1 + p4;
        seq_t s3 = p2 + p3;

        if (s1 == s2 && s2 == s3) {
            /* append p0 to the cache */
            if (_seq_len == _seq_cap) {
                size_t new_cap = _seq_cap ? _seq_cap * 2 : 64;
                seq_t *tmp = seq_realloc(_seq_buf, _seq_cap, new_cap);
                _seq_buf = tmp;
                _seq_cap = new_cap;
            }
            _seq_buf[_seq_len++] = p0;
        }
        _next_k++; /* advance to next starting index */
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n < 1, but guard */
    _ensure_cache(n);
    return _seq_buf[n - 1];
}
