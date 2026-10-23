/*
 * A049550  Primes p such that x^18 = 2 has a solution mod p.
 * https://oeis.org/A049550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   this function returns the n-th prime p for which 2 is an 18th
 *            power residue modulo p, i.e. the existence of x with x^18 ≡ 2
 *            (mod p). The first term is p = 2
 * Range:     u8 n <= 16, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

static size_t _found_len = 0;
static size_t _found_cap = 0;
static seq_t * _found = NULL;
static idx_t _next_check = 2;

/* Euclidean gcd for idx_t */
static inline idx_t _gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Test whether p (p≥3) satisfies the condition. */
static inline bool _good_prime(idx_t p)
{
    if (p == 2) return true;
    idx_t g = _gcd_idx(18, p-1);
    idx_t exp = (p-1) / g;
    seq_t res = mod_pow((seq_t)2, (uint64_t)exp, (seq_t)p);
    return res == (seq_t)1;
}

seq_t a(idx_t n)
{
    /* Ensure the static storage is initialized */
    if (_found == NULL) {
        _found_cap = 16;
        _found = (seq_t *)malloc(_found_cap * sizeof(seq_t));
        _found_len = 0;
    }

    /* Generate primes until we have n of them that satisfy the condition */
    while (_found_len < n) {
        if (is_prime(_next_check)) {
            if (_good_prime(_next_check)) {
                if (_found_len == _found_cap) {
                    _found_cap <<= 1;
                    _found = (seq_t *)realloc(_found, _found_cap * sizeof(seq_t));
                }
                _found[_found_len++] = (seq_t)_next_check;
            }
        }
        _next_check++;
    }

    return _found[n-1];
}
