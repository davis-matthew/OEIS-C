/*
 * A207869  a(n) = Z(n,-1), where Z(n,x) is the n-th Zeckendorf polynomial.
 * https://oeis.org/A207869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{k} (-1)^k, where the sum runs over the indices of
 *            Fibonacci numbers F_k (with F_2=1, F_3=2, ...) appearing in the
 *            Zeckendorf representation of n. This equals Z(n,-1)
 * Range:     i8 n <= 83, i16 n <= 83, i32 n <= 83, i64 n <= 83, i128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* for bool type if desired, but not strictly needed */

static bool _zinit = false;
#define _ZFIB_MAX 94          /* enough to cover all 64‑bit values */
static idx_t _zfib[_ZFIB_MAX];
static seq_t _zsign[_ZFIB_MAX];

static void _zinit_fib(void)
{
    _zfib[0] = 0;            /* unused */
    _zfib[1] = 1;            /* F1 */
    _zfib[2] = 1;            /* F2 */
    /* compute F_i for i = 3..93.  F_94 would overflow 64‑bit unsigned. */
    for (int i = 3; i < _ZFIB_MAX; i++) {
        _zfib[i] = _zfib[i-1] + _zfib[i-2];
    }
    /* pre‑compute signs (-1)^i for i >= 2 */
    for (int i = 2; i < _ZFIB_MAX; i++) {
        _zsign[i] = (i & 1) ? (seq_t)(-1) : (seq_t)1;
    }
    _zinit = true;
}

seq_t a(idx_t n)
{
    if (!_zinit) _zinit_fib();

    seq_t sum = 0;
    idx_t rem = n;

    /* Iterate over Fibonacci indices from largest to 2. */
    for (int i = _ZFIB_MAX - 1; i >= 2; --i) {
        if (rem >= _zfib[i]) {
            rem -= _zfib[i];
            sum += _zsign[i];
        }
    }
    return sum;
}
