/*
 * A115601  a(n) = numerator of b(n), where b(1) = 1, b(n+1) = Sum_{k=1..n}
 *          b(k)^((-1)^(n-k+1)).
 * https://oeis.org/A115601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = c(n-1)/gcd(c(n-1),c(n-2)) where c(n) =
 *            \prod_{k=1}^{\lfloor n/2\rfloor} (3*2^{\,n-2k} - 1)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 11, u64 n <= 17, u128 n <= 23,
 *            big any n
 * Helpers:   _a_small, gcd, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/pow2.h"
#include "../utils/_a_small.h"

/* choose implementation depending on the size of seq_t */
static const size_t _seqt_size = sizeof(seq_t);

/* -------------------------------------------------------------
   Large seq_t implementation (arbitrary‑precision or uint128_t).
   Caches the values of c(n) and uses the generic gcd routine.
   ------------------------------------------------------------- */
static seq_t * _c_cache = NULL;
static size_t _c_len   = 0;
static size_t _c_cap   = 0;

static void _c_ensure(size_t need)
{
    if (need <= _c_len) return;

    if (_c_cap == 0) {
        _c_cap = need > 1 ? need : 1;
        _c_cache = (seq_t*)malloc(_c_cap * sizeof(seq_t));
        _c_cache[0] = (seq_t)1;
        _c_len   = 1;
    }
    if (need > _c_cap) {
        while (_c_cap < need) _c_cap <<= 1;
        _c_cache = (seq_t*)realloc(_c_cache, _c_cap * sizeof(seq_t));
    }

    for (size_t i = _c_len; i < need; ++i) {
        idx_t N = (idx_t)i;                 /* N = i */
        seq_t prod = (seq_t)1;
        for (idx_t k = 1; k <= N / 2; ++k) {
            idx_t exp = N - 2 * k;
            /* 3*2^exp - 1 */
            seq_t term = (seq_t)3 * pow2(exp) - (seq_t)1;
            prod = prod * term;
        }
        _c_cache[i] = prod;
    }
    _c_len = need;
}

static seq_t _a_large(idx_t n)
{
    idx_t n1 = n - 1;
    idx_t n2 = n - 2;
    size_t need = (size_t)(n1 > n2 ? n1 : n2) + 1;   /* ensure up to max(n1,n2) */
    _c_ensure(need);

    seq_t c1 = _c_cache[(size_t)n1];
    seq_t c2 = _c_cache[(size_t)n2];
    seq_t g  = gcd(c1, c2);
    return c1 / g;
}

/* -------------------------------------------------------------
   Public entry point.
   ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* handle trivial cases */
    if (n <= 2) return (seq_t)1;

    if (_seqt_size <= 8)  /* uint64_t or smaller */
        return _a_small(n);
    else                 /* arbitrary‑precision or uint128_t */
        return _a_large(n);
}
