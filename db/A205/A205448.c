/*
 * A205448  Ordered differences of even-indexed Fibonacci numbers.
 * https://oeis.org/A205448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = F_{2i} - F_{2j} * where i is the least integer satisfying
 *            (i-1)*i/2 ≥ n, * and j = n - (i-2)*(i-1)/2
 * Range:     u8 n <= 15, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <assert.h>
#include <stdbool.h>

static seq_t fib(idx_t n) {
    if (n <= 2) return (seq_t)1;
    seq_t f1 = 1, f2 = 1, f3;
    for (idx_t i = 3; i <= n; ++i) {
        f3 = f1 + f2;
        f1 = f2;
        f2 = f3;
    }
    return f2;
}

/* Find the minimal i >= 2 with (i-1)*i/2 >= n.  */
static inline idx_t find_i(idx_t n)
{
    idx_t low = 2;
    idx_t high = 4;
    /* Increase high until the condition is met. */
    while ((unsigned __int128)(high - 1) * high / 2 < n) {
        high <<= 1;
    }
    /* Binary search for the exact i. */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)(mid - 1) * mid / 2 >= n) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    return low;
}

seq_t a(idx_t n)
{
    assert(n >= 1);

    idx_t i = find_i(n);
    /* Number of terms before the block with index i. */
    idx_t prev_terms = ((unsigned __int128)(i - 2) * (i - 1)) / 2;
    idx_t j = n - prev_terms;   /* 1 ≤ j < i */

    idx_t fi = 2 * i;
    idx_t fj = 2 * j;

    seq_t s_i = fib(fi);
    seq_t s_j = fib(fj);

    return s_i - s_j;
}
