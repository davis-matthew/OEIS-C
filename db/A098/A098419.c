/*
 * A098419  Primes not occurring in prime triples (p,q,r) with p<q<r=p+6.
 * https://oeis.org/A098419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by generating primes and skipping those that belong to a prime triple (p,q,r) with p<q<r=p+6). */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

static bool _in_prime_triple(idx_t p)
{
    /* p as smallest: (p, p+2, p+6) */
    if (is_prime(p + 2) && is_prime(p + 6))
        return true;
    /* p as smallest: (p, p+4, p+6) */
    if (is_prime(p + 4) && is_prime(p + 6))
        return true;
    /* p as middle: (p-2, p, p+4) */
    if (p >= 2 && is_prime(p - 2) && is_prime(p + 4))
        return true;
    /* p as middle: (p-4, p, p+2) */
    if (p >= 4 && is_prime(p - 4) && is_prime(p + 2))
        return true;
    /* p as largest: (p-6, p-2, p) */
    if (p >= 6 && is_prime(p - 6) && is_prime(p - 2))
        return true;
    /* p as largest: (p-6, p-4, p) */
    if (p >= 6 && is_prime(p - 6) && is_prime(p - 4))
        return true;
    return false;
}

static idx_t      _last_checked  = 1;   /* largest number examined so far   */
static seq_t     *_seq_buf       = NULL;/* dynamic array of sequence values */
static size_t     _seq_len       = 0;   /* number of values stored          */
static size_t     _seq_cap       = 0;   /* allocated capacity of _seq_buf    */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined for offset 0 */

    while (n > _seq_len) {
        /* Find the next prime after _last_checked */
        do {
            ++_last_checked;
        } while (!is_prime(_last_checked));

        if (!_in_prime_triple(_last_checked)) {
            /* store the prime */
            if (_seq_len == _seq_cap) {
                size_t new_cap = _seq_cap ? _seq_cap << 1 : 8;
                seq_t *tmp = (seq_t *)realloc(_seq_buf, new_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0; /* allocation failure */
                _seq_buf = tmp;
                _seq_cap = new_cap;
            }
            _seq_buf[_seq_len++] = (seq_t)_last_checked;
        }
        /* continue until we have enough terms */
    }
    return _seq_buf[n - 1];
}
