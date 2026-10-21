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
 * Helpers:   get_prime, seq_alloc_new_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* External utilities provided by the build environment */
#include "../utils/get_prime.h"          /* uint64_t get_prime(idx_t k); */
#include "../utils/seq_alloc_new_array.h"/* seq_t* seq_alloc_new_array(size_t n); */
#include "../utils/seq_realloc.h"        /* seq_t* seq_realloc(seq_t *old, size_t old_cap, size_t new_cap); */

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
