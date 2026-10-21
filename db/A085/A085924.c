/*
 * A085924  If k = product (p_i)^(r_i), where p_i are primes in increasing
 *          order, then k is a member if concatenation of r_i as decimal
 *          numbers forms a palindrome.
 * https://oeis.org/A085924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, less, easy, nonn
 * Formula:   a(n) = n-th integer k (starting from k=1) such that the
 *            concatenation of the exponents in the prime factorization of k
 *            is a palindrome in base 10
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation generates the sequence by scanning integers
   sequentially and memoising results.  It uses a dynamic array for
   the cache and a small table of primes for trial division.
   The algorithm works in O(N log N) time for the first N terms
   and easily handles n up to about 10 000.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------ */
/* Dynamic array utilities for seq_t (value type)                */
/* ------------------------------------------------------------ */
static size_t _seq_cache_len = 0, _seq_cache_cap = 0;
static seq_t *_seq_cache = NULL;

/* Append a value to the cache, growing it if necessary.    */
static void _cache_append(seq_t val) {
    if (_seq_cache_len == _seq_cache_cap) {
        size_t new_cap = _seq_cache_cap == 0 ? 64 : _seq_cache_cap * 2;
        seq_t *newp = (seq_t *)realloc(_seq_cache, new_cap * sizeof(seq_t));
        if (!newp) abort();            /* allocation failure */
        _seq_cache = newp;
        _seq_cache_cap = new_cap;
    }
    _seq_cache[_seq_cache_len++] = val;
}

/* ------------------------------------------------------------ */
/* Small prime table for trial division                         */
/* ------------------------------------------------------------ */
static size_t _prime_len = 0, _prime_cap = 0;
static uint64_t *_prime_table = NULL;

/* Ensure that _prime_table contains all primes up to at least 'limit'.
   The table is grown incrementally.                                             */
static void _ensure_primes_upto(uint64_t limit) {
    if (_prime_len == 0) {
        // initialise with first two primes
        _prime_view:
        _prime_len = 0;
        if (_prime_cap < 2) {
            _prime_cap = 64;
            uint64_t *tmp = (uint64_t *)realloc(_prime_table, _prime_cap * sizeof(uint64_t));
            if (!tmp) abort();
            _prime_table = tmp;
        }
        _prime_table[_prime_len++] = 2;
        _prime_table[_prime_len++] = 3;
    }
    /*
     * We need primes up to the square‑root of the numbers we will test.
     * Extend the table if the largest prime we have is still too small.
     */
    while (_prime_len > 0 && _prime_table[_prime_len - 1] * _prime_table[_prime_len - 1] <= limit) {
        /* Find next prime candidate (odd numbers only). */
        uint64_t cand = _prime_table[_prime_len - 1] + 2;
        while (true) {
            bool isprime = true;
            for (size_t i = 0; i < _prime_len; ++i) {
                uint64_t p = _prime_table[i];
                if (p * p > cand) break;
                if (cand % p == 0) { isprime = false; break; }
            }
            if (isprime) {
                if (_prime_len == _prime_cap) {
                    size_t new_cap = _prime_cap == 0 ? 64 : _prime_cap * 2;
                    uint64_t *tmp = (uint64_t *)realloc(_prime_table, new_cap * sizeof(uint64_t));
                    if (!tmp) abort();
                    _prime_table = tmp;
                    _prime_cap = new_cap;
                }
                _prime_table[_prime_len++] = cand;
                break;
            }
            cand += 2;   /* next odd */
        }
    }
}

/* ------------------------------------------------------------ */
/* Test whether k satisfies the palindrome‑exponent condition  */
/* ------------------------------------------------------------ */
static bool _is_member(uint64_t k) {
    if (k == 1) return true;        /* empty exponent list is a palindrome */

    /* Extract exponents */
    uint8_t exps[32];
    size_t ecount = 0;

    uint64_t tmp = k;
    _ensure_primes_upto(tmp);       /* primes up to sqrt(k) are available */

    for (size_t i = 0; i < _prime_len && _prime_table[i] * _prime_table[i] <= tmp; ++i) {
        uint64_t p = _prime_table[i];
        if (tmp % p != 0) continue;
        uint8_t cnt = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++cnt;
        }
        exps[ecount++] = cnt;
    }
    if (tmp > 1) {                 /* remaining prime factor with exponent 1 */
        exps[ecount++] = 1;
    }

    /* Build concatenated decimal string of exponents in a temporary buffer. */
    char buf[128];
    size_t len = 0;
    for (size_t i = 0; i < ecount; ++i) {
        uint8_t val = exps[i];
        /* Convert val to characters (always non‑negative). */
        char digs[4];
        size_t dlen = 0;
        do {
            digs[dlen++] = '0' + (char)(val % 10);
            val /= 10;
        } while (val);
        /* Append reversed digits to buf. */
        for (size_t d = 0; d < dlen; ++d) {
            buf[len++] = digs[dlen - 1 - d];
        }
    }

    /* Check palindrome */
    for (size_t l = 0, r = len; l < r; ++l, --r) {
        if (buf[l] != buf[r - 1]) return false;
    }
    return true;
}

/* ------------------------------------------------------------ */
/* Main sequence function                                        */
/* ------------------------------------------------------------ */
seq_t a(idx_t n) {
    /* Grow cache until it contains at least n+1 terms. */
    static uint64_t next_candidate = 1;     /* integer to investigate next */
    while (_seq_cache_len <= n) {
        if (_is_member(next_candidate)) {
            _cache_append(next_candidate);
        }
        ++next_candidate;
    }
    return _seq_cache[n];
}
