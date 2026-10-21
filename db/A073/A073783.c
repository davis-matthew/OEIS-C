/*
 * A073783  First differences of composite numbers.
 * https://oeis.org/A073783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A002808(n+1) – A002808(n) (difference between consecutive
 *            composite numbers)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool is_prime(idx_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (idx_t d = 3; d <= x / d; d += 2) {
        if (x % d == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* not expected, but safe */

    idx_t composite_count = 0;
    idx_t cur = 4;   /* first composite */
    idx_t comp_n = 0;
    idx_t comp_np1 = 0;

    while (true) {
        if (!is_prime(cur)) {
            composite_count++;
            if (composite_count == n) comp_n = cur;
            if (composite_count == n + 1) {
                comp_np1 = cur;
                break;
            }
        }
        cur++;
    }

    return (seq_t)(comp_np1 - comp_n);
}
