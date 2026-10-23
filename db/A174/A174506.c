/*
 * A174506  Continued fraction expansion for exp( Sum_{n>=1} 1/(n*A014448(n))
 *          ), where A014448(n) = (2+sqrt(5))^n + (2-sqrt(5))^n.
 * https://oeis.org/A174506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  cofr, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A174506: Continued fraction expansion for exp( Sum_{n>=1} 1/(n*A014448(n)) ).
 *
 *  Formula used:
 *    a(3k)   = 1
 *    a(3k-2) = A014448(2k-1) - 1
 *    a(3k-1) = A014448(2k)   - 1
 *
 *  where A014448(m) = (2+√5)^m + (2-√5)^m is an integer satisfying
 *
 *        b(0)=2,  b(1)=4,
 *        b(m)=4*b(m-1)+b(m-2)   (for m>=2)
 *
 *  The sequence values are computed exactly using the provided seq_t type,
 *  which supports ordinary arithmetic operators.
 */
#include <stdlib.h>
#include <stdint.h>

/* Memoized table of A014448(m) values */
static seq_t * _a014448_cache = NULL;
static size_t  _a014448_cache_size = 0;

/* Compute A014448(n) (n>=0).  Uses memoization for speed. */
static seq_t _a014448_compute(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)4;

    /* Initialise cache with first two terms if it does not exist */
    if (_a014448_cache_size == 0) {
        size_t init_sz = 2;
        _a014448_cache      = (seq_t*)malloc(init_sz * sizeof(seq_t));
        _a014448_cache_size = init_sz;
        _a014448_cache[0]   = (seq_t)2;
        _a014448_cache[1]   = (seq_t)4;
    }

    /* Extend the cache if necessary */
    if ((size_t)n >= _a014448_cache_size) {
        size_t new_sz = (size_t)n + 1;
        seq_t *new_arr = (seq_t*)realloc(_a014448_cache, new_sz * sizeof(seq_t));
        _a014448_cache = new_arr;
        for (size_t i = _a014448_cache_size; i <= (size_t)n; ++i) {
            _a014448_cache[i] = _a014448_cache[i-1] * (seq_t)4 + _a014448_cache[i-2];
        }
        _a014448_cache_size = new_sz;
    }
    return _a014448_cache[(size_t)n];
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* a(3k) = 1 */
    if (n % 3 == 0) {
        return (seq_t)1;
    }
    /* a(3k-2) = A014448(2k-1) - 1 */
    if (n % 3 == 1) {
        idx_t k = (n + 2) / 3;
        seq_t val = _a014448_compute(2*k - 1);
        return val - (seq_t)1;
    }
    /* a(3k-1) = A014448(2k) - 1 */
    {
        idx_t k = (n + 1) / 3;
        seq_t val = _a014448_compute(2*k);
        return val - (seq_t)1;
    }
}
