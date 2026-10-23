/*
 * A040665  Continued fraction for sqrt(692).
 * https://oeis.org/A040665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   periodic continued fraction of sqrt(692) a(0) = floor(sqrt(692))
 *            = 26 a(1..) = terms of the continued fraction, repeating with
 *            period 14: 3,3,1,2,1,1,12,1,1,2,1,3,3,52
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

/* Integer square root: floor(sqrt(n)) for non‑negative n.
   Uses only integer arithmetic, no floating‑point. */
static size_t _isqrt(size_t n) {
    size_t low = 0, high = n;
    while (low < high) {
        size_t mid = (low + high + 1) / 2;
        if (mid <= n / mid)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Internal storage for the sequence.  The period is 14,
   but we allocate a bit more for safety. */
static seq_t _period[64];
static size_t _period_len = 0;
static seq_t _a0 = 0;
static int _initialized = 0;

/* Initialise the period and the initial term. */
static void _init(void) {
    /* Compute a0 = floor(sqrt(692)) */
    size_t a0i = _isqrt(692);
    _a0 = (seq_t)a0i;

    /* Compute continued fraction for sqrt(692) */
    size_t m = 0;
    size_t d = 1;
    size_t a = a0i;          /* first a is a0 already computed */
    while (1) {
        m = d * a - m;
        d = (692 - m * m) / d;
        a = (a0i + m) / d;

        /* Store the term */
        _period[_period_len++] = (seq_t)a;

        /* Period ends when the term equals 2*a0 */
        if (a == 2 * a0i)
            break;
    }
    _initialized = 1;
}

/* Public function defined by the problem statement. */
seq_t a(idx_t n) {
    if (!_initialized)
        _init();

    if (n == 0)
        return _a0;
    /* Repeat the periodic part */
    return _period[(size_t)(n - 1) % _period_len];
}
