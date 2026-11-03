/*
 * A233314  Primes p such that p+6 and p+12 are composite.
 * https://oeis.org/A233314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 69 terms through n = 981 (2026-11-03)
 * Helpers:   is_prime_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A233314 by searching primes and checking that p+6 and p+12 are composite */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/next_prime.h"
#include "../utils/is_prime_u64.h"

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

static uint64_t _last_checked = 1;      /* last examined TINK prime candidate */
static seq_t   *_terms = NULL;          /* cache of found terms */
static size_t  _len = 0;                /* number of stored terms */
static size_t  _cap = 0;                /* allocated capacity */

/* Return the n-th term of A233314 (1‑based index).  The function is
   cache‑aware and works correctly when called in any order.  It uses
   an auxiliary routine seq_realloc for memory management so that it
   compiles with both fixed‑size and arbitrary‑precision seq_t. */
seq_t a(idx_t n)
{
    /* Grow the cache until we have at least n terms. */
    while (_len < (size_t)n) {
        /* Get next prime after the last one examined. */
        uint64_t p = next_prime(_last_checked);
        _last_checked = p;

        /* Check the two required conditions. */
        bool ok = !is_prime_big((seq_t)(p + 6ULL)) &&
                  !is_prime_big((seq_t)(p + 12ULL));

        if (ok) {
            /* Reallocate the array if necessary. */
            if (_len >= _cap) {
                size_t new_cap = _cap ? _cap * 2 : 64;
                seq_t *tmp = seq_realloc(_terms, _cap, new_cap);
                if (!tmp) {
                    /* Out of memory – return a zero value. */
                    return (seq_t)0;
                }
                _terms = tmp;
                _cap   = new_cap;
            }
            /* Store the term. */
            _terms[_len++] = (seq_t)p;
        }
    }
    return _terms[n - 1];
}
