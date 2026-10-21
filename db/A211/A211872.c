/*
 * A211872  For each triprime (A014612) less than or equal to n, sum the
 *          positive integers less than or equal to the number of divisors of
 *          the triprime.
 * https://oeis.org/A211872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n} [Omega(i) = 3] * (d(i)*(d(i)+1)/2) where
 *            Omega(i) is the total number of prime factors of i with
 *            multiplicity, and d(i) is the number of divisors of i
 * Range:     u8 n <= 51, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Auxiliary data that is cached across calls. */
static idx_t _cached_n = 0;        /* highest n processed */
static size_t _cap = 0;            /* allocation size of arrays */
static uint16_t * _omega = NULL;   /* Ω(i) for i <= _cached_n */
static uint16_t * _dcnt  = NULL;   /* d(i) for i <= _cached_n */

/* Compute Ω(i) and d(i) for i in (from .. to] where to <= n. */
static void _extend_cache(idx_t from, idx_t to)
{
    /* Ensure allocation is large enough. */
    size_t needed = (size_t)to + 1;          /* indices 0 .. to */
    if (needed > _cap) {
        size_t new_cap = needed > 2 * _cap ? needed : 2 * _cap;
        if (new_cap == 0) new_cap = needed; /* first allocation */
        uint16_t *tmp;

        tmp = (uint16_t*)realloc(_omega, new_cap * sizeof(uint16_t));
        if (tmp == NULL) { /* allocation failed */
            /* In a real program you would handle the error. */
            abort();
        }
        _omega = tmp;

        tmp = (uint16_t*)realloc(_dcnt, new_cap * sizeof(uint16_t));
        if (tmp == NULL) {
            abort();
        }
        _dcnt = tmp;

        _cap = new_cap;
    }

    for (idx_t i = from + 1; i <= to; ++i) {
        uint64_t x = (uint64_t)i;
        uint16_t omega_i = 0;
        uint16_t d_i = 1;

        /* Trial division up to sqrt(x).  For n <= 10000 this is fast. */
        for (uint64_t p = 2; p * p <= x; ++p) {
            if (x % p == 0) {
                uint16_t exp = 0;
                while (x % p == 0) {
                    x /= p;
                    ++exp;
                }
                omega_i += exp;
                d_i *= (exp + 1);
            }
        }
        if (x > 1) {          /* remaining prime factor */
            omega_i += 1;
            d_i *= 2;
        }

        _omega[i] = omega_i;
        _dcnt[i] = d_i;
    }
    _cached_n = to;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* Extend cache if necessary. */
    if (n > _cached_n) {
        _extend_cache(_cached_n, n);
    }

    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        if (_omega[i] == 3) {
            seq_t d = (seq_t)_dcnt[i];
            seq_t contrib = d * (d + 1) / 2;
            sum += contrib;
        }
    }
    return sum;
}
