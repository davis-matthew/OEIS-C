/*
 * A205838  The least number j such that 2 divides s(k)-s(j), where
 *          k(n)=A205720(n).
 * https://oeis.org/A205838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = j of the n-th pair (k,j), j<k, with s(k)-s(j) even,
 *            s(m)=Fibonacci(m+1); pairs ordered by k then j (computed by
 *            direct enumeration)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static idx_t cnt_pairs_for_k(idx_t k)
{
    if (k <= 2) return 0;
    idx_t m = k - 1;
    idx_t c2 = (m >= 2) ? ((m - 2) / 3 + 1) : 0; /* j = 2 mod 3 */
    if (k % 3 == 2) return c2;
    return m - c2;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    idx_t sum = 0;
    for (idx_t k = 3; ; ++k) {
        idx_t c = cnt_pairs_for_k(k);
        if (sum + c >= n) {
            idx_t i = n - sum; /* 1-based */
            idx_t j;
            if (k % 3 == 2)
                j = 2 + (i - 1) * 3;
            else
                j = (3 * i) / 2;
            return (seq_t)j;
        }
        sum += c;
    }
}
