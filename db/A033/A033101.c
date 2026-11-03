/*
 * A033101  Number of 4's when n is written in base b for 2<=b<=n+1.
 * https://oeis.org/A033101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of 4's in the base‑b representation of n for * all
 *            bases b with 2 ≤ b ≤ n+1
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Validated: partial, pass, 88 terms through n = 88 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Count how many times digit 4 appears in the base‑b representation
 * of n.  idx_t is an unsigned integer type, seq_t is an unsigned
 * or arbitrary‑precision integer type. */
static inline seq_t count_in_base(idx_t n, idx_t b)
{
    seq_t cnt = 0;
    while (n > 0) {
        if ((n % b) == 4) {
            cnt += 1;
        }
        n /= b;
    }
    return cnt;
}

/* Required function: computes a(n) as defined above. */
seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t b = 2; b <= n + 1; ++b) {
        total += count_in_base(n, b);
    }
    return total;
}
