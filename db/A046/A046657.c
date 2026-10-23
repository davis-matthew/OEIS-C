/*
 * A046657  a(n) = A002088(n)/2.
 * https://oeis.org/A046657
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/2 + Sum_{i<j<=n, gcd(i, j) = 1} i/j Equivalent
 *            recurrence (for n ≥ 3): a(2) = 1 a(n) = a(n-1) + φ(n)/2 where
 *            φ(n) is Euler's totient function
 * Range:     u8 n <= 40, u16 n <= 656, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static helper to precompute totients and sequence values up to a given index */
static idx_t _max_computed = 0;
static seq_t * _a_cache   = NULL;   /* a(n) values */
static uint64_t * _phi = NULL;      /* φ(n) values */

/* recompute tables up to n (inclusive) */
static void _ensure(idx_t n)
{
    if (n <= _max_computed) return;

    size_t newsize = (size_t)(n + 1);

    _a_cache = (seq_t*)realloc(_a_cache, newsize * sizeof(seq_t));
    _phi    = (uint64_t*)realloc(_phi,   newsize * sizeof(uint64_t));
    if (!_a_cache || !_phi) exit(1);

    /* initialize φ array */
    for (idx_t i = 0; i <= n; ++i) _phi[i] = (uint64_t)i;
    for (idx_t i = 2; i <= n; ++i) {
        if (_phi[i] == (uint64_t)i) {          /* i is prime */
            for (idx_t j = i; j <= n; j += i)
                _phi[j] -= _phi[j] / i;
        }
    }

    /* compute a(n) values */
    if (n >= 0) _a_cache[0] = 0;
    if (n >= 1) _a_cache[1] = 0;
    if (n >= 2) _a_cache[2] = (seq_t)1;
    for (idx_t i = 3; i <= n; ++i) {
        _a_cache[i] = _a_cache[i-1] + (seq_t)(_phi[i] / 2);
    }

    _max_computed = n;
}

/* required function */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;   /* below the offset; value unspecified */
    _ensure(n);
    return _a_cache[n];
}
