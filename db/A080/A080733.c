/*
 * A080733  Smallest distance from n to a squarefree number.
 * https://oeis.org/A080733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min (|n-k| : k is squarefree)
 * Range:     u8 n <= 9527, u16 n <= 9527, u32 n <= 9527, u64 n <= 9527,
 *            u128 n <= 9527, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool is_squarefree(idx_t n)
{
    if (n <= 1) return true;
    idx_t d, root;
    for (d = 2; (root = d * d) <= n; ++d) {
        int count = 0;
        while (n % d == 0) {
            n /= d;
            ++count;
            if (count >= 2) return false;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    idx_t k = 0;
    while (1) {
        if (k == 0) {
            if (is_squarefree(n)) return (seq_t)k;
        } else {
            if (n >= k && is_squarefree(n - k)) return (seq_t)k;
            if (is_squarefree(n + k))       return (seq_t)k;
        }
        ++k;
    }
}
