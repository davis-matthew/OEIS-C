/*
 * A018688  Divisors of 860.
 * https://oeis.org/A018688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   _sort_divs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Divisors of 860, sorted ascending */

#include <stdlib.h>
#include "../utils/_sort_divs.h"

/* Prime factorization of 860 = 2^2 * 5 * 43 */
static const uint64_t _primes[3] = { 2ULL, 5ULL, 43ULL };
static const unsigned _exps[3]   = { 2U, 1U, 1U };
static const size_t _num_primes   = 3;

/* Cached divisor array */
static seq_t *_divs = NULL;       /* pointer to divisor list */
static size_t _div_count = 0;     /* number of divisors (12) */

/* Recursive helper to generate all divisors */
static void _gen_divs(const uint64_t *primes,
                      const unsigned *exps,
                      size_t          num,
                      size_t          idx,
                      seq_t           val,
                      seq_t          *out,
                      size_t         *pos)
{
    if (idx == num) {
        out[(*pos)++] = val;
        return;
    }
    seq_t base = (seq_t)primes[idx];
    seq_t mult = (seq_t)1;
    for (unsigned i = 0; i <= exps[idx]; ++i) {
        _gen_divs(primes, exps, num, idx + 1, val * mult, out, pos);
        mult *= base;
    }
}

/* Initialize the divisor array on first use */
static void _init_divs(void)
{
    /* total number of divisors: product of (exponent+1) */
    size_t cnt = 1;
    for (size_t i = 0; i < _num_primes; ++i)
        cnt *= (size_t)(_exps[i] + 1);

    _div_count = cnt;
    _divs = (seq_t *)malloc(cnt * sizeof(seq_t));
    if (!_divs) {
        _div_count = 0;
        return;            /* memory failure – callers will get 0 */
    }

    size_t pos = 0;
    _gen_divs(_primes, _exps, _num_primes, 0, (seq_t)1, _divs, &pos);

    /* sort ascending */
    _sort_divs(_divs, _div_count);
}

/* Public function required by the build system */
seq_t a(idx_t n)
{
    if (!_divs)
        _init_divs();

    if (_divs == NULL || n < 1 || (size_t)n > _div_count)
        return 0;           /* out of bounds or uninitialized */

    return _divs[n - 1];
}
