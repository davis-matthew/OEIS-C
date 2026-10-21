/*
 * A072885  Primes p with a prime number of digits in all base b with 1 <= b
 *          <= p.
 * https://oeis.org/A072885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, full, fini
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   _has_composite_digit_length, _is_prime_small, ensure_capacity,
 *            is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: generate primes p such that the number of digits of p in every base
   b with 1 <= b <= p is a prime.  This is OEIS A072885. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>          /* for sqrt in case size_t > integer, only used for cast */
#include "../utils/is_prime.h"
#include "../utils/ensure_capacity.h"
#include "../utils/_has_composite_digit_length.h"
#include "../utils/_is_prime_small.h"

/* Main sequence generator with memoisation. */
seq_t a(idx_t n)
{
    static bool          _initialized = false;
    static idx_t         _next_candidate = 2;
    static seq_t       * _seq_cache = NULL;
    static size_t        _seq_cap   = 0;
    static size_t        _seq_len   = 0;

    if (!_initialized)
    {
        _seq_cap = 1024;
        _seq_cache = (seq_t *)malloc(_seq_cap * sizeof(seq_t));
        _seq_len = 0;
        _initialized = true;
    }

    while (_seq_len < n)
    {
        if (is_prime(_next_candidate))
        {
            idx_t p = _next_candidate;
            if (!_has_composite_digit_length(p))
            {
                ensure_capacity(_seq_len + 1, &_seq_cap,
                        (void **)&_seq_cache, sizeof(seq_t));
                _seq_cache[_seq_len++] = (seq_t)p;
            }
        }
        ++_next_candidate;
    }

    return _seq_cache[n - 1];
}
