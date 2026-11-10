/*
 * A214796  Numbers n such that n-8=A214795(n).
 * https://oeis.org/A214796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th number m with A214795(m) = m-8, i.e. smallest k>=2
 *            with m | Fibonacci(k-1)+21 equals m-8; found by search
 * Range:     u8 n <= 7, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 36 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t A214796_add_mod(idx_t a, idx_t b, idx_t mod)
{
    if (a >= mod - b)
        return a - (mod - b);
    return a + b;
}

/* A214795(m) == m-8 ? (m >= 10) */
static int A214796_test(idx_t m)
{
    idx_t f_prev = 0;
    idx_t f_curr = 1 % m;
    idx_t c21 = 21 % m;
    idx_t target = m - 9; /* index j = k-1 */
    for (idx_t j = 1; j <= target; ++j)
    {
        if (A214796_add_mod(f_curr, c21, m) == 0)
            return j == target;
        idx_t next = A214796_add_mod(f_prev, f_curr, m);
        f_prev = f_curr;
        f_curr = next;
    }
    return 0;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t m = 10;; ++m)
    {
        if (A214796_test(m))
        {
            ++count;
            if (count == n)
                return (seq_t)m;
        }
    }
}
